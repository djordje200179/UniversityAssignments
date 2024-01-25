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
		var weekAgo = new Date(System.currentTimeMillis() - 7L * 24 * 60 * 60 * 1000);
		var monthAgo = new Date(System.currentTimeMillis() - 30L * 24 * 60 * 60 * 1000);

		return new WelcomePageCounters(
			studentsRepository.count(), teachersRepository.countByActivatedIsTrue(),
			classesRepository.countAllByTimeBeforeAndConfirmedIsTrueAndCancelledIsFalse(new Timestamp(weekAgo.getTime())),
			classesRepository.countAllByTimeBeforeAndConfirmedIsTrueAndCancelledIsFalse(new Timestamp(monthAgo.getTime()))
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
}
