package com.djordjemilanovic.backend.services;

import com.djordjemilanovic.backend.models.*;
import com.djordjemilanovic.backend.repositories.ClassesRepository;
import com.djordjemilanovic.backend.repositories.SubjectsRepository;
import com.djordjemilanovic.backend.repositories.TeacherSubjectsRepository;
import com.djordjemilanovic.backend.repositories.TeachersRepository;
import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.stream.Collectors;

@Service
@AllArgsConstructor
public class TeachersService {
	private final TeacherSubjectsRepository teacherSubjectRepository;
	private final TeachersRepository teachersRepository;
	private final ClassesRepository classesRepository;
	private final SubjectsRepository subjectsRepository;

	public Collection<SubjectEntity> getAllSubjects() {
		return subjectsRepository.findAll();
	}

	public static class SubjectAlreadyExistsException extends Exception {
		public SubjectAlreadyExistsException(String message) {
			super("Предмет већ постоји");
		}
	}

	public SubjectEntity addSubject(String name) throws SubjectAlreadyExistsException {
		var existing = subjectsRepository.findById(name);
		if (existing.isPresent())
			throw new SubjectAlreadyExistsException("Предмет већ постоји");

		var subject = new SubjectEntity(name);
		return subjectsRepository.save(subject);
	}

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

	public void blockTeacher(String username) {
		var teacher = teachersRepository.findById(username).orElseThrow();
		teacher.setActivated(false);
		teacher.setBlocked(true);
		teachersRepository.save(teacher);
	}

	public void acceptTeacher(String username) {
		var teacher = teachersRepository.findById(username).orElseThrow();
		teacher.setActivated(true);
		teachersRepository.save(teacher);
	}

	public static record Rating(String comment, Integer rating) {
	}

	public Collection<Rating> getRatings(String username) {
		var classes = classesRepository.findAllByTeacherUsername(username);

		var ratings = new ArrayList<Rating>();
		for (var c : classes) {
			if (c.getStudentRating() == null)
				continue;

			ratings.add(new Rating(c.getStudentComment(), c.getStudentRating()));
		}

		return ratings;
	}
}
