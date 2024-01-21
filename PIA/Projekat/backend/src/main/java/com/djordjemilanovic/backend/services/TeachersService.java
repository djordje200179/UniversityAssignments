package com.djordjemilanovic.backend.services;

import com.djordjemilanovic.backend.models.ClassEntity;
import com.djordjemilanovic.backend.models.StudentEntity;
import com.djordjemilanovic.backend.models.TeacherEntity;
import com.djordjemilanovic.backend.models.TeacherSubjectEntity;
import com.djordjemilanovic.backend.repositories.ClassesRepository;
import com.djordjemilanovic.backend.repositories.TeacherSubjectsRepository;
import com.djordjemilanovic.backend.repositories.TeachersRepository;
import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

import java.util.Collection;
import java.util.stream.Collectors;

@Service
@AllArgsConstructor
public class TeachersService {
	private final TeacherSubjectsRepository teacherSubjectRepository;
	private final TeachersRepository teachersRepository;
	private final ClassesRepository classesRepository;

	public Collection<TeacherSubjectEntity> getEnrollments() {
		return teacherSubjectRepository.findAllByIdTeacherActivatedIsTrue();
	}

	public Collection<TeacherSubjectEntity> getEnrollments(int schoolYear) {
		Collection<TeacherEntity> teachers;
		if (schoolYear < 5)
			teachers = teachersRepository.findAllByTeachesLowerElementaryIsTrueAndActivatedIsTrue();
		else if (schoolYear < 9)
			teachers = teachersRepository.findAllByTeachesUpperElementaryIsTrueAndActivatedIsTrue();
		else if (schoolYear < 13)
			teachers = teachersRepository.findAllByTeachesHighIsTrueAndActivatedIsTrue();
		else
			throw new IllegalArgumentException("Invalid school year");

		return teacherSubjectRepository.findAllByIdTeacherIn(teachers);
	}

	public Collection<TeacherSubjectEntity> getEnrollments(String username) {
		return teacherSubjectRepository.findAllByIdTeacherUsername(username);
	}

	public static class TeacherOccupiedException extends Exception {
		public TeacherOccupiedException(String message) {
			super(message);
		}
	}

	public Collection<StudentEntity> getStudents(String username) {
		var allClasses = classesRepository.findAllByTeacherUsername(username);
		return allClasses.stream().map(ClassEntity::getStudent).collect(Collectors.toSet());
	}
}
