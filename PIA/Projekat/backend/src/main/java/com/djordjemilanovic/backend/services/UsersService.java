package com.djordjemilanovic.backend.services;

import com.djordjemilanovic.backend.models.*;
import com.djordjemilanovic.backend.repositories.*;
import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Optional;

@AllArgsConstructor
@Service
public class UsersService {
	private final UsersRepository usersRepository;
	private final UsersInfoRepository usersInfoRepository;
	private final StudentsRepository studentsRepository;
	private final TeachersRepository teachersRepository;
	private final TeacherSubjectsRepository teacherSubjectRepository;
	private final SubjectsRepository subjectsRepository;
	private final NotificationsRepository notificationsRepository;

	public Optional<UserInfoEntity> find(String username) {
		return usersInfoRepository.findById(username);
	}

	public Optional<UserInfoEntity> find(String username, String password) {
		var passwordHash = password;

		var optionalUser = usersRepository.findByUsernameAndPasswordHash(username, password);
		if (optionalUser.isEmpty())
			return Optional.empty();

		var userInfoOpt = usersInfoRepository.findById(username);
		if (userInfoOpt.isEmpty())
			return Optional.of(new UserInfoEntity(username, UserInfoEntity.Role.ADMIN));

		var userInfo = userInfoOpt.get();
		if (userInfo.getStudent() != null)
			userInfo.setRole(UserInfoEntity.Role.STUDENT);
		else if (userInfo.getTeacher() != null)
			userInfo.setRole(UserInfoEntity.Role.TEACHER);

		return Optional.of(userInfo);
	}

	public Optional<UserInfoEntity> findBackup(String username, String securityAnswer) {
		var userInfoOpt = usersInfoRepository.findByUsernameAndSecurityAnswer(username, securityAnswer);
		if (userInfoOpt.isEmpty())
			return Optional.empty();

		var userInfo = userInfoOpt.get();
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
			super("Корисник већ постоји");
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

		for (var subjectName : subjects) {
			var optSubjectEntity = subjectsRepository.findById(subjectName);
			SubjectEntity subject;
			if (optSubjectEntity.isEmpty()) {
				subject = new SubjectEntity(subjectName);
				subjectsRepository.save(subject);
			} else {
				subject = optSubjectEntity.get();
			}

			var teacherSubject = new TeacherSubjectEntity(teacher, subject);
			teacherSubjectRepository.save(teacherSubject);
		}

		return teacher;
	}

	public Collection<TeacherEntity> getTeachers() {
		return teachersRepository.findAll();
	}

	public Collection<StudentEntity> getStudents() {
		return studentsRepository.findAll();
	}

	public Collection<TeacherEntity> getTeacherRequests() {
		return teachersRepository.findAllByActivatedIsFalseAndBlockedIsFalse();
	}

	public Collection<NotificationEntity> getNotifications(String username) {
		var notifications = notificationsRepository.findAllByReceiverUsername(username);

		var copiedNotifications = new ArrayList<NotificationEntity>();
		for (var notification : notifications) {
			var notificationCopy = new NotificationEntity(notification.getReceiver(), notification.getMessage());
			notificationCopy.setSeen(notification.isSeen());
			copiedNotifications.add(notificationCopy);
		}

		for (var notification : notifications) {
			notification.setSeen(true);
			notificationsRepository.save(notification);
		}

		return copiedNotifications;
	}

	public StudentEntity incrementSchoolYear(String username) {
		var student = studentsRepository.findById(username).orElse(null);

		switch (student.getSchoolType()) {
		case ELEMENTARY:
			if (student.getSchoolYear() == 8) {
				student.setSchoolYear(1);
				student.setSchoolType(StudentEntity.SchoolType.GYMNASIUM);
			} else
				student.setSchoolYear(student.getSchoolYear() + 1);

			studentsRepository.save(student);
			break;
		default:
			if (student.getSchoolYear() == 4)
				break;

			student.setSchoolYear(student.getSchoolYear() + 1);
			studentsRepository.save(student);
			break;
		}

		return student;
	}
}
