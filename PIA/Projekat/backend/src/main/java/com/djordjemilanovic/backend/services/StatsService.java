package com.djordjemilanovic.backend.services;

import com.djordjemilanovic.backend.models.TeacherEntity;
import com.djordjemilanovic.backend.models.UserInfoEntity;
import com.djordjemilanovic.backend.repositories.StudentsRepository;
import com.djordjemilanovic.backend.repositories.SubjectsRepository;
import com.djordjemilanovic.backend.repositories.TeachersRepository;
import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

import java.util.*;
import java.util.stream.Collectors;

@Service
@AllArgsConstructor
public class StatsService {
	private final StudentsRepository studentsRepository;
	private final TeachersRepository teachersRepository;
	private final SubjectsRepository subjectsRepository;

	public record WelcomePageCounters(
		long numberOfStudents,
		long numberOfTeachers,

		int numberOfClassesLastWeek,
		int numberOfClassesLastMonth
	) {}

	public WelcomePageCounters getWelcomePageCounters() {
		return new WelcomePageCounters(
			studentsRepository.count(), teachersRepository.countByActivatedIsTrue(),
			10, 20
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
