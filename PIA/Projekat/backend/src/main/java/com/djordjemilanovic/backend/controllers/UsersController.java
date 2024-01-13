package com.djordjemilanovic.backend.controllers;

import com.djordjemilanovic.backend.models.UserEntity;
import com.djordjemilanovic.backend.models.UserInfoEntity;
import com.djordjemilanovic.backend.services.UsersService;
import org.springframework.web.bind.annotation.*;

import java.util.Collection;
import java.util.Optional;

@RestController
@CrossOrigin(origins = "*", allowedHeaders = "*")
@RequestMapping("/users")
public class UsersController {
	private final UsersService usersService;

	public UsersController(UsersService usersService) {
		this.usersService = usersService;
	}

	public record SignInRequest(String username, String password) {}

	@PostMapping("/sign-in")
	public Optional<UserInfoEntity> signIn(@RequestBody SignInRequest user) {
		return usersService.find(user.username, user.password);
	}

	public record SignUpRequestUserInfo(
		String firstName, String lastName,
		String gender,
		String securityQuestion, String securityAnswer,
		String address,
		String phoneNumber,
		String email
	) {}

	public record SignUpStudentRequest(
		SignInRequest credentials,
		SignUpRequestUserInfo info,
		String schoolType,
		int schoolYear
	) {}

	@PostMapping("/sign-up/student")
	public Optional<UserInfoEntity> signUpStudent(@RequestBody SignUpStudentRequest request) {
		var gender = UserInfoEntity.Gender.valueOf(request.info.gender.toUpperCase());

		return usersService.createStudent(
			request.credentials.username, request.credentials.password,
			request.info.securityQuestion, request.info.securityAnswer,
			request.info.firstName, request.info.lastName, gender,
			request.info.address, request.info.phoneNumber, request.info.email,
			request.schoolType, request.schoolYear
		);
	}

	public record SignUpTeacherRequest(
		SignInRequest credentials,
		SignUpRequestUserInfo info,
		Collection<String> subjects
	) {}

	@PostMapping("/sign-up/teacher")
	public Optional<UserInfoEntity> signUpTeacher(@RequestBody SignUpTeacherRequest request) {
		var gender = UserInfoEntity.Gender.valueOf(request.info.gender.toUpperCase());

		return usersService.createTeacher(
				request.credentials.username, request.credentials.password,
				request.info.securityQuestion, request.info.securityAnswer,
				request.info.firstName, request.info.lastName, gender,
				request.info.address, request.info.phoneNumber, request.info.email,
				request.subjects
		);
	}
}
