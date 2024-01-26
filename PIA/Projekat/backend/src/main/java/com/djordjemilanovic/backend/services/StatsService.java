package com.djordjemilanovic.backend.services;

import com.djordjemilanovic.backend.models.TeacherEntity;
import com.djordjemilanovic.backend.models.UserInfoEntity;
import com.djordjemilanovic.backend.repositories.ClassesRepository;
import com.djordjemilanovic.backend.repositories.StudentsRepository;
import com.djordjemilanovic.backend.repositories.SubjectsRepository;
import com.djordjemilanovic.backend.repositories.TeachersRepository;
import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

import java.sql.Timestamp;
import java.util.*;
import java.util.stream.Collectors;

@Service
@AllArgsConstructor
public class StatsService {
	private final StudentsRepository studentsRepository;
	private final TeachersRepository teachersRepository;
	private final SubjectsRepository subjectsRepository;
	private final ClassesRepository classesRepository;

	public record WelcomePageCounters(
		long numberOfStudents,
		long numberOfTeachers,

		long numberOfClassesLastWeek,
		long numberOfClassesLastMonth
	) {}

	public WelcomePageCounters getWelcomePageCounters() {
		var weekAgo = new Timestamp(System.currentTimeMillis() - 7L * 24 * 60 * 60 * 1000);
		var monthAgo = new Timestamp(System.currentTimeMillis() - 30L * 24 * 60 * 60 * 1000);
		var currentTime = new Timestamp(System.currentTimeMillis());

		return new WelcomePageCounters(
			studentsRepository.count(), teachersRepository.countByActivatedIsTrue(),
			classesRepository.findAllByTimeBetweenAndConfirmedIsTrueAndCancelledIsFalse(weekAgo, currentTime).size(),
			classesRepository.findAllByTimeBetweenAndConfirmedIsTrueAndCancelledIsFalse(monthAgo, currentTime).size()
		);
	}

	public Map<String, Map<String, Long>> getSubjectAgeCounters() {
		var teacherAgeGroups = new HashMap<String, Collection<TeacherEntity>>();
		teacherAgeGroups.put("Нижи основци", teachersRepository.findAllByTeachesLowerElementaryIsTrueAndActivatedIsTrue());
		teacherAgeGroups.put("Виши основци", teachersRepository.findAllByTeachesUpperElementaryIsTrueAndActivatedIsTrue());
		teacherAgeGroups.put("Средњошколци", teachersRepository.findAllByTeachesHighIsTrueAndActivatedIsTrue());

		var subjects = subjectsRepository.findAll();

		var results = new HashMap<String, Map<String, Long>>();
		for (var ageGroupEntry : teacherAgeGroups.entrySet()) {
			var subjectsMap = new HashMap<String, Long>();

			for (var subject : subjects)
				subjectsMap.put(subject.getName(), 0L);

			for (var teacher : ageGroupEntry.getValue()) {
				for (var teacherSubjects : teacher.getSubjects()) {
					var subjectName = teacherSubjects.getId().getSubject().getName();
					subjectsMap.put(subjectName, subjectsMap.get(subjectName) + 1);
				}
			}

			results.put(ageGroupEntry.getKey(), subjectsMap);
		}

		return results;
	}

	public Map<UserInfoEntity.Gender, Long> getStudentCountByGender() {
		return studentsRepository.findAll().stream().collect(Collectors.groupingBy(student -> student.getInfo().getGender(), Collectors.counting()));
	}

	public Map<UserInfoEntity.Gender, Long> getTeacherCountByGender() {
		return teachersRepository.findAll().stream().collect(Collectors.groupingBy(student -> student.getInfo().getGender(), Collectors.counting()));
	}

	public Collection<Long> getClassesPerDays() {
		var counters = new ArrayList<Long>();
		for (int i = 0; i < 7; i++)
			counters.add(0L);

		var start = new Timestamp(123, 0, 1, 0, 0, 0, 0);
		var end = new Timestamp(123, 11, 31, 23, 59, 59, 999999999);

		var allClasses = classesRepository.findAllByTimeBetweenAndConfirmedIsTrueAndCancelledIsFalse(start, end);
		for (var c : allClasses) {
			var day = c.getTime().getDay();
			counters.set(day, counters.get(day) + 1);
		}

		return counters;
	}

	public static record TopTeacherInfo (
		TeacherEntity teacher,
		ArrayList<Integer> classesPerMonth
	) {}

	public Collection<TopTeacherInfo> getTopTeachers() {
		var start = new Timestamp(123, 0, 1, 0, 0, 0, 0);
		var end = new Timestamp(123, 11, 31, 23, 59, 59, 999999999);

		var allClasses = classesRepository.findAllByTimeBetweenAndConfirmedIsTrueAndCancelledIsFalse(start, end);

		var teacherClasses = new HashMap<TeacherEntity, ArrayList<Integer>>();
		for (var c : allClasses) {
			var teacher = c.getTeacher();
			if (!teacherClasses.containsKey(teacher)) {
				var perMonthCounters = new ArrayList<Integer>();
				for (int i = 0; i < 12; i++)
					perMonthCounters.add(0);

				teacherClasses.put(teacher, perMonthCounters);
			}

			var month = c.getTime().getMonth();
			teacherClasses.get(teacher).set(month, teacherClasses.get(teacher).get(month) + 1);
		}

		var topTeachers = teacherClasses.entrySet().stream().sorted((a, b) -> {
			var aSum = a.getValue().stream().mapToInt(Integer::intValue).sum();
			var bSum = b.getValue().stream().mapToInt(Integer::intValue).sum();

			return bSum - aSum;
		}).limit(10).toList();

		var results = new ArrayList<TopTeacherInfo>();
		for (var teacher : topTeachers)
			results.add(new TopTeacherInfo(teacher.getKey(), teacher.getValue()));

		return results;
	}
}
