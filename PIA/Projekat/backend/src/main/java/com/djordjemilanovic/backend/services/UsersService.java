package com.djordjemilanovic.backend.services;

import com.djordjemilanovic.backend.models.StudentEntity;
import com.djordjemilanovic.backend.models.TeacherEntity;
import com.djordjemilanovic.backend.models.UserEntity;
import com.djordjemilanovic.backend.models.UserInfoEntity;
import com.djordjemilanovic.backend.repositories.StudentsRepository;
import com.djordjemilanovic.backend.repositories.TeachersRepository;
import com.djordjemilanovic.backend.repositories.UsersInfoRepository;
import com.djordjemilanovic.backend.repositories.UsersRepository;
import org.springframework.stereotype.Service;

import java.util.Collection;
import java.util.List;
import java.util.Optional;

@Service
public class UsersService {
	private final UsersRepository usersRepository;
	private final UsersInfoRepository usersInfoRepository;
	private final StudentsRepository studentsRepository;
	private final TeachersRepository teachersRepository;

	public UsersService(
			UsersRepository usersRepository, UsersInfoRepository usersInfoRepository,
			StudentsRepository studentsRepository, TeachersRepository teachersRepository) {
		this.usersRepository = usersRepository;
		this.usersInfoRepository = usersInfoRepository;
		this.studentsRepository = studentsRepository;
		this.teachersRepository = teachersRepository;
	}

	public List<UserEntity> getAll() {
		return usersRepository.findAll();
	}

	public Optional<UserInfoEntity> find(String username, String password) {
		var passwordHash = password;

		var optionalUser = usersRepository.findByUsernameAndPasswordHash(username, password);
		if (optionalUser.isEmpty())
			return Optional.empty();

		var user = optionalUser.get();

		return usersInfoRepository.findById(username);
	}

	private Optional<UserInfoEntity> createUser(
		String username, String password,
		String securityQuestion, String securityAnswer,
		String firstName, String lastName, UserInfoEntity.Gender gender,
		String address, String phoneNumber, String emailAddress
	) {
		if (usersRepository.findById(username).isPresent())
			return Optional.empty();

		if (usersInfoRepository.findByEmailAddress(emailAddress).isPresent())
			return Optional.empty();

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

		return Optional.of(userInfo);
	}

	public Optional<UserInfoEntity> createStudent(
			String username, String password,
			String securityQuestion, String securityAnswer,
			String firstName, String lastName, UserInfoEntity.Gender gender,
			String address, String phoneNumber, String emailAddress,
			String schoolType, int schoolYear
	) {
		var userInfo = createUser(
				username, password,
				securityQuestion, securityAnswer,
				firstName, lastName, gender,
				address, phoneNumber, emailAddress
		);

		if (userInfo.isEmpty())
			return Optional.empty();

		var studentInfo = new StudentEntity(username, schoolType, schoolYear);
		studentsRepository.save(studentInfo);

		return userInfo;
	}

	public Optional<UserInfoEntity> createTeacher(
			String username, String password,
			String securityQuestion, String securityAnswer,
			String firstName, String lastName, UserInfoEntity.Gender gender,
			String address, String phoneNumber, String emailAddress,
			Collection<String> subjects
	) {
		var userInfo = createUser(
				username, password,
				securityQuestion, securityAnswer,
				firstName, lastName, gender,
				address, phoneNumber, emailAddress
		);

		if (userInfo.isEmpty())
			return Optional.empty();

		var teacherInfo = new TeacherEntity(username);
		teachersRepository.save(teacherInfo);

		return userInfo;
	}
}
