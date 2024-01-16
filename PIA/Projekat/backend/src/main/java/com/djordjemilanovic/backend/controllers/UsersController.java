package com.djordjemilanovic.backend.controllers;

import com.djordjemilanovic.backend.models.StudentEntity;
import com.djordjemilanovic.backend.models.TeacherEntity;
import com.djordjemilanovic.backend.models.UserInfoEntity;
import com.djordjemilanovic.backend.services.FileStorageService;
import com.djordjemilanovic.backend.services.UsersService;
import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.AllArgsConstructor;
import lombok.SneakyThrows;
import org.springframework.core.io.Resource;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import java.util.Collection;

@RestController
@CrossOrigin(origins = "*", allowedHeaders = "*")
@RequestMapping("/users")
@AllArgsConstructor
public class UsersController {
	private final UsersService usersService;
	private final ObjectMapper mapper;
	private final FileStorageService fileStorageService;

	public record SignInRequest(String username, String password) {
	}

	@PostMapping("/sign-in")
	public ResponseEntity<UserInfoEntity> signIn(@RequestBody SignInRequest request) {
		try {
			var user = usersService.find(request.username, request.password);
			return user.map(ResponseEntity::ok).orElse(ResponseEntity.notFound().build());
		} catch (Exception e) {
			e.printStackTrace();
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

	@SneakyThrows
	@PostMapping(value = "/sign-up/student", consumes = MediaType.MULTIPART_FORM_DATA_VALUE, produces = MediaType.APPLICATION_JSON_VALUE)
	public ResponseEntity<UserInfoEntity> signUpStudent(
			@RequestPart(value = "profile-image", required = false) MultipartFile profileImage,
			@RequestPart("data") String jsonData) {
		var request = mapper.readValue(jsonData, SignUpStudentRequest.class);

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

			if (profileImage != null)
				fileStorageService.saveProfileImage(profileImage, request.credentials.username);
			else
				fileStorageService.saveGenericProfileImage(request.credentials.username);

			return ResponseEntity.ok(student.getInfo());
		} catch (UsersService.UserAlreadyExistsException e) {
			return ResponseEntity.status(HttpStatus.CONFLICT).build();
		} catch (Exception e) {
			e.printStackTrace();
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

	@SneakyThrows
	@PostMapping(value = "/sign-up/teacher", consumes = MediaType.MULTIPART_FORM_DATA_VALUE, produces = MediaType.APPLICATION_JSON_VALUE)
	public ResponseEntity<UserInfoEntity> signUpTeacher(
			@RequestPart("biography") MultipartFile biography,
			@RequestPart(value = "profile-image", required = false) MultipartFile profileImage,
			@RequestPart("data") String jsonData) {
		var request = mapper.readValue(jsonData, SignUpTeacherRequest.class);

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

			fileStorageService.saveBiography(biography, request.credentials.username);
			if (profileImage != null)
				fileStorageService.saveProfileImage(profileImage, request.credentials.username);
			else
				fileStorageService.saveGenericProfileImage(request.credentials.username);

			return ResponseEntity.ok(teacher.getInfo());
		} catch (UsersService.UserAlreadyExistsException e) {
			return ResponseEntity.status(HttpStatus.CONFLICT).build();
		} catch (Exception e) {
			e.printStackTrace();
			return ResponseEntity.internalServerError().build();
		}
	}

	@GetMapping("/student/{username}")
	public ResponseEntity<StudentEntity> getStudent(@PathVariable String username) {
		try {
			var student = usersService.getStudent(username);
			return ResponseEntity.ok(student);
		} catch (Exception e) {
			e.printStackTrace();
			return ResponseEntity.internalServerError().build();
		}
	}

	@GetMapping("/teacher/{username}")
	public ResponseEntity<TeacherEntity> getTeacher(@PathVariable String username) {
		try {
			var teacher = usersService.getTeacher(username);
			return ResponseEntity.ok(teacher);
		} catch (Exception e) {
			e.printStackTrace();
			return ResponseEntity.internalServerError().build();
		}
	}

	@SneakyThrows
	@GetMapping(value="/profile-image/{username}", produces = MediaType.IMAGE_PNG_VALUE)
	public @ResponseBody byte[] getProfileImage(@PathVariable String username) {
		var resource = fileStorageService.loadProfileImage(username);
		return resource.getInputStream().readAllBytes();
	}
}
