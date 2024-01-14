package com.djordjemilanovic.backend.services;

import com.djordjemilanovic.backend.models.*;
import com.djordjemilanovic.backend.repositories.*;
import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

import java.util.Collection;
import java.util.List;
import java.util.Optional;

@AllArgsConstructor
@Service
public class UsersService {
	private final UsersRepository usersRepository;
	private final UsersInfoRepository usersInfoRepository;
	private final StudentsRepository studentsRepository;
	private final TeachersRepository teachersRepository;
	private final TeacherSubjectsRepository teacherSubjectRepository;

	public List<UserEntity> getAll() {
		return usersRepository.findAll();
	}

	public Optional<UserInfoEntity> find(String username, String password) {
		var passwordHash = password;

		var optionalUser = usersRepository.findByUsernameAndPasswordHash(username, password);
		if (optionalUser.isEmpty())
			return Optional.empty();

		return usersInfoRepository.findById(username);
	}

	public static class UserAlreadyExistsException extends Exception {
		public UserAlreadyExistsException() {
			super("User already exists");
		}
	}

	private UserInfoEntity createUser(
		String username, String password,
		String securityQuestion, String securityAnswer,
		String firstName, String lastName, UserInfoEntity.Gender gender,
		String address, String phoneNumber, String emailAddress
	) throws UserAlreadyExistsException {
		if (usersRepository.findById(username).isPresent())
			throw new UserAlreadyExistsException();

		if (usersInfoRepository.findByEmailAddress(emailAddress).isPresent())
			throw new UserAlreadyExistsException();

		var passwordHash = password;

		var user = new UserEntity(username, passwordHash);
		var userInfo = new UserInfoEntity(
				username,
				securityQuestion, securityAnswer,
				firstName, lastName, gender,
				address, phoneNumber, emailAddress
		);

		usersRepository.save(user);
		usersInfoRepository.save(userInfo);

		return userInfo;
	}

	public StudentEntity createStudent(
			String username, String password,
			String securityQuestion, String securityAnswer,
			String firstName, String lastName, UserInfoEntity.Gender gender,
			String address, String phoneNumber, String emailAddress,
			StudentEntity.SchoolType schoolType, int schoolYear
	) throws UserAlreadyExistsException {
		createUser(
			username, password,
			securityQuestion, securityAnswer,
			firstName, lastName, gender,
			address, phoneNumber, emailAddress
		);

		var student = new StudentEntity(username, schoolType, schoolYear);
		studentsRepository.save(student);

		return student;
	}

	public TeacherEntity createTeacher(
			String username, String password,
			String securityQuestion, String securityAnswer,
			String firstName, String lastName, UserInfoEntity.Gender gender,
			String address, String phoneNumber, String emailAddress,
			Collection<String> subjects
	) throws UserAlreadyExistsException {
		createUser(
			username, password,
			securityQuestion, securityAnswer,
			firstName, lastName, gender,
			address, phoneNumber, emailAddress
		);

		var teacher = new TeacherEntity(username);
		teachersRepository.saveAndFlush(teacher);

		for (var subject : subjects) {
			var teacherSubject = new TeacherSubjectEntity(teacher, subject);
			teacherSubjectRepository.save(teacherSubject);
		}

		return teacher;
	}

	public Collection<TeacherSubjectEntity> getTeacherEnrollments() {
		return teacherSubjectRepository.findAll();
	}
}
