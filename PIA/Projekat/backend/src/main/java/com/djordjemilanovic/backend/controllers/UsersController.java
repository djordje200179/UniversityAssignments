package com.djordjemilanovic.backend.controllers;

import com.djordjemilanovic.backend.models.StudentEntity;
import com.djordjemilanovic.backend.models.TeacherEntity;
import com.djordjemilanovic.backend.models.UserInfoEntity;
import com.djordjemilanovic.backend.services.UsersService;
import lombok.AllArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Collection;

@RestController
@CrossOrigin(origins = "*", allowedHeaders = "*")
@RequestMapping("/users")
@AllArgsConstructor
public class UsersController {
	private final UsersService usersService;

	public record SignInRequest(String username, String password) {
	}

	@PostMapping("/sign-in")
	public ResponseEntity<UserInfoEntity> signIn(@RequestBody SignInRequest request) {
		try {
			var user = usersService.find(request.username, request.password);
			return user.map(ResponseEntity::ok).orElse(ResponseEntity.notFound().build());
		} catch (Exception e) {
			return ResponseEntity.internalServerError().build();
		}
	}

	public record SignUpRequestUserInfo(
			String firstName, String lastName,
			String gender,
			String securityQuestion, String securityAnswer,
			String address,
			String phoneNumber,
			String emailAddress
	) {
	}

	public record SignUpStudentRequest(
			SignInRequest credentials,
			SignUpRequestUserInfo info,
			String schoolType,
			int schoolYear
	) {
	}

	@PostMapping("/sign-up/student")
	public ResponseEntity<UserInfoEntity> signUpStudent(@RequestBody SignUpStudentRequest request) {
		var gender = UserInfoEntity.Gender.valueOf(request.info.gender.toUpperCase());
		var schoolType = StudentEntity.SchoolType.valueOf(request.schoolType.toUpperCase());

		try {
			var student = usersService.createStudent(
					request.credentials.username, request.credentials.password,
					request.info.securityQuestion, request.info.securityAnswer,
					request.info.firstName, request.info.lastName, gender,
					request.info.address, request.info.phoneNumber, request.info.emailAddress,
					schoolType, request.schoolYear
			);

			return ResponseEntity.ok(student.getInfo());
		} catch (UsersService.UserAlreadyExistsException e) {
			return ResponseEntity.status(HttpStatus.CONFLICT).build();
		} catch (Exception e) {
			return ResponseEntity.internalServerError().build();
		}
	}

	public record SignUpTeacherRequest(
			SignInRequest credentials,
			SignUpRequestUserInfo info,
			Collection<String> subjects,

			boolean teachesLowerElementary,
			boolean teachesUpperElementary,
			boolean teachesHigh
	) {
	}

	@PostMapping("/sign-up/teacher")
	public ResponseEntity<UserInfoEntity> signUpTeacher(@RequestBody SignUpTeacherRequest request) {
		var gender = UserInfoEntity.Gender.valueOf(request.info.gender.toUpperCase());

		try {
			var teacher = usersService.createTeacher(
					request.credentials.username, request.credentials.password,
					request.info.securityQuestion, request.info.securityAnswer,
					request.info.firstName, request.info.lastName, gender,
					request.info.address, request.info.phoneNumber, request.info.emailAddress,
					request.subjects,
					request.teachesLowerElementary, request.teachesUpperElementary, request.teachesHigh
			);

			return ResponseEntity.ok(teacher.getInfo());
		} catch (UsersService.UserAlreadyExistsException e) {
			return ResponseEntity.status(HttpStatus.CONFLICT).build();
		} catch (Exception e) {
			return ResponseEntity.internalServerError().build();
		}
	}

	@GetMapping("/student/{username}")
	public ResponseEntity<StudentEntity> getStudent(@PathVariable String username) {
		try {
			var student = usersService.getStudent(username);
			return ResponseEntity.ok(student);
		} catch (Exception e) {
			return ResponseEntity.internalServerError().build();
		}
	}

	@GetMapping("/teacher/{username}")
	public ResponseEntity<TeacherEntity> getTeacher(@PathVariable String username) {
		try {
			var teacher = usersService.getTeacher(username);
			return ResponseEntity.ok(teacher);
		} catch (Exception e) {
			return ResponseEntity.internalServerError().build();
		}
	}
}
