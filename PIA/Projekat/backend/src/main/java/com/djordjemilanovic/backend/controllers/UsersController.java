package com.djordjemilanovic.backend.controllers;

import com.djordjemilanovic.backend.models.NotificationEntity;
import com.djordjemilanovic.backend.models.StudentEntity;
import com.djordjemilanovic.backend.models.TeacherEntity;
import com.djordjemilanovic.backend.models.UserInfoEntity;
import com.djordjemilanovic.backend.services.FileStorageService;
import com.djordjemilanovic.backend.services.UsersService;
import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.AllArgsConstructor;
import lombok.SneakyThrows;
import org.springframework.core.io.Resource;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import java.util.Collection;
import java.util.Map;

@RestController
@CrossOrigin(origins = "http://localhost:4200", allowedHeaders = "*")
@RequestMapping("/users")
@AllArgsConstructor
public class UsersController {
	private final UsersService usersService;
	private final ObjectMapper mapper;
	private final FileStorageService fileStorageService;

	public record SignInRequest(String username, String password) {
	}

	@PostMapping("/sign-in-admin")
	public ResponseEntity<UserInfoEntity> adminSignIn(
			@RequestHeader(value = HttpHeaders.AUTHORIZATION, required = false) String auth
	) {
		var headers = new HttpHeaders();
		headers.add("Access-Control-Allow-Credentials", "true");
		headers.add("WWW-Authenticate", "Basic realm=\"Admin\"");

		if (auth == null) {
			return ResponseEntity
					.status(HttpStatus.UNAUTHORIZED)
					.headers(headers)
					.build();
		}

		var credentials = auth.split(" ")[1];
		var decoded = new String(java.util.Base64.getDecoder().decode(credentials));
		var username = decoded.split(":")[0];
		var password = decoded.split(":")[1];

		var user = usersService.find(username, password);
		if (user.isPresent() && user.get().getRole() == UserInfoEntity.Role.ADMIN)
			return ResponseEntity.status(HttpStatus.OK).headers(headers).body(user.get());
		else
			return ResponseEntity.status(HttpStatus.UNAUTHORIZED).headers(headers).build();
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
		try (var in = resource.getInputStream()) {
			return in.readAllBytes();
		}
	}

	@SneakyThrows
	@PostMapping(value ="/profile-image/{username}", consumes = MediaType.MULTIPART_FORM_DATA_VALUE)
	public UserInfoEntity updateProfileImage(
			@PathVariable String username,
			@RequestPart("profile-image") MultipartFile profileImage
	) {
		fileStorageService.saveProfileImage(profileImage, username);

		return usersService.find(username).get();
	}

	@GetMapping("/teachers")
	public Collection<TeacherEntity> getTeachers() {
		return usersService.getTeachers();
	}

	@GetMapping("/students")
	public Collection<StudentEntity> getStudents() {
		return usersService.getStudents();
	}

	@GetMapping("/teachers/requests")
	public Collection<TeacherEntity> getTeacherRequests() {
		return usersService.getTeacherRequests();
	}

	@GetMapping("/notifications/{username}")
	public Collection<NotificationEntity> getNotifications(@PathVariable String username) {
		return usersService.getNotifications(username);
	}

	@PutMapping("/student/{username}/increment")
	public StudentEntity incrementSchoolYear(@PathVariable String username) {
		return usersService.incrementSchoolYear(username);
	}

	@GetMapping("/security-question/{username}")
	public ResponseEntity<String> getSecurityQuestion(@PathVariable String username) {
		var securityQuestion = usersService.find(username).map(UserInfoEntity::getSecurityQuestion);
		return securityQuestion.map(ResponseEntity::ok).orElse(ResponseEntity.notFound().build());
	}



	@PostMapping("/sign-in-backup")
	public ResponseEntity<UserInfoEntity> signInBackup(@RequestBody SignInRequest request) {
		try {
			var user = usersService.findBackup(request.username, request.password);
			return user.map(ResponseEntity::ok).orElse(ResponseEntity.notFound().build());
		} catch (Exception e) {
			e.printStackTrace();
			return ResponseEntity.internalServerError().build();
		}
	}

	public record PasswordChangeRequest (
			String newPassword,
			String authAnswer,
			String authType
	) {}

	@PutMapping("/password/{username}")
	public ResponseEntity<UserInfoEntity> changePassword(
			@PathVariable String username, @RequestBody PasswordChangeRequest request
	) {
		var user = usersService.changePassword(username, request.newPassword, request.authAnswer, request.authType);
		return user.map(ResponseEntity::ok).orElse(ResponseEntity.badRequest().build());
	}

	@PutMapping("/update")
	public ResponseEntity<UserInfoEntity> updateUserInfo(@RequestBody UserInfoEntity user) {
		try {
			var updatedUser = usersService.updateUser(user);
			return ResponseEntity.ok(updatedUser);
		} catch (UsersService.UserAlreadyExistsException e) {
			return ResponseEntity.status(HttpStatus.CONFLICT).build();
		} catch (Exception e) {
			e.printStackTrace();
			return ResponseEntity.internalServerError().build();
		}

	}
}
