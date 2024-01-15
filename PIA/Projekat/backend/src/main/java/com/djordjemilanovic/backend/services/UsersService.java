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

		var userInfo = usersInfoRepository.findById(username).get();
		if (userInfo.getStudent() != null)
			userInfo.setRole(UserInfoEntity.Role.STUDENT);
		else if (userInfo.getTeacher() != null)
			userInfo.setRole(UserInfoEntity.Role.TEACHER);

		return Optional.of(userInfo);
	}

	public StudentEntity getStudent(String username) {
		return studentsRepository.findById(username).orElse(null);
	}

	public TeacherEntity getTeacher(String username) {
		return teachersRepository.findById(username).orElse(null);
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
		String address, String phoneNumber, String emailAddress,
		UserInfoEntity.Role role
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
				address, phoneNumber, emailAddress,
				role
		);

		usersRepository.save(user);
		usersInfoRepository.saveAndFlush(userInfo);

		return userInfo;
	}

	public StudentEntity createStudent(
			String username, String password,
			String securityQuestion, String securityAnswer,
			String firstName, String lastName, UserInfoEntity.Gender gender,
			String address, String phoneNumber, String emailAddress,
			StudentEntity.SchoolType schoolType, int schoolYear
	) throws UserAlreadyExistsException {
		var userInfo = createUser(
			username, password,
			securityQuestion, securityAnswer,
			firstName, lastName, gender,
			address, phoneNumber, emailAddress,
			UserInfoEntity.Role.STUDENT
		);

		var student = new StudentEntity(username, schoolType, schoolYear);
		studentsRepository.save(student);
		student.setInfo(userInfo);

		return student;
	}

	public TeacherEntity createTeacher(
			String username, String password,
			String securityQuestion, String securityAnswer,
			String firstName, String lastName, UserInfoEntity.Gender gender,
			String address, String phoneNumber, String emailAddress,
			Collection<String> subjects,
			boolean teachesLowerElementary, boolean teachesUpperElementary, boolean teachesHigh
	) throws UserAlreadyExistsException {
		var userInfo = createUser(
			username, password,
			securityQuestion, securityAnswer,
			firstName, lastName, gender,
			address, phoneNumber, emailAddress,
			UserInfoEntity.Role.TEACHER
		);

		var teacher = new TeacherEntity(
			username,
			teachesLowerElementary, teachesUpperElementary, teachesHigh
		);
		teachersRepository.saveAndFlush(teacher);
		teacher.setInfo(userInfo);

		for (var subject : subjects) {
			var teacherSubject = new TeacherSubjectEntity(teacher, subject);
			teacherSubjectRepository.save(teacherSubject);
		}

		return teacher;
	}
}
