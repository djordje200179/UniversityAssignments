package com.djordjemilanovic.backend.services;

import com.djordjemilanovic.backend.models.TeacherEntity;
import com.djordjemilanovic.backend.models.TeacherSubjectEntity;
import com.djordjemilanovic.backend.repositories.TeacherSubjectsRepository;
import com.djordjemilanovic.backend.repositories.TeachersRepository;
import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

import java.util.Collection;

@Service
@AllArgsConstructor
public class TeachersService {
	private final TeacherSubjectsRepository teacherSubjectRepository;
	private final TeachersRepository teachersRepository;

	public Collection<TeacherSubjectEntity> getEnrollments() {
		return teacherSubjectRepository.findAll();
	}

	public Collection<TeacherSubjectEntity> getEnrollments(int schoolYear) {
		Collection<TeacherEntity> teachers;
		if (schoolYear < 5)
			teachers = teachersRepository.findAllByTeachesLowerElementaryIsTrue();
		else if (schoolYear < 9)
			teachers = teachersRepository.findAllByTeachesUpperElementaryIsTrue();
		else if (schoolYear < 13)
			teachers = teachersRepository.findAllByTeachesHighIsTrue();
		else
			throw new IllegalArgumentException("Invalid school year");

		return teacherSubjectRepository.findAllByIdTeacherIn(teachers);
	}

	public Collection<TeacherSubjectEntity> getEnrollments(String username) {
		return teacherSubjectRepository.findAllByIdTeacherUsername(username);
	}
}
