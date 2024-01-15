package com.djordjemilanovic.backend.models;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonValue;
import jakarta.persistence.*;
import lombok.*;
import org.hibernate.annotations.ColumnTransformer;
import org.hibernate.annotations.NotFound;
import org.hibernate.annotations.NotFoundAction;

@Setter
@Getter
@Entity
@EqualsAndHashCode
@NoArgsConstructor
@Table(name = "users_info", schema = "pia")
public class UserInfoEntity {
	public enum Gender {
		FEMALE, MALE;

		@JsonValue
		public String toLowerCase() {
			return toString().toLowerCase();
		}
	}

	@Id
	@Column(name = "username")
	private String username;
	@Basic
	@JsonIgnore
	@Column(name = "security_question")
	private String securityQuestion;
	@Basic
	@JsonIgnore
	@Column(name = "security_answer")
	private String securityAnswer;
	@Basic
	@Column(name = "first_name")
	private String firstName;
	@Basic
	@Column(name = "last_name")
	private String lastName;
	@Basic
	@Column(name = "gender")
	@ColumnTransformer(read = "UPPER(gender)", write = "LOWER(?)")
	@Enumerated(EnumType.STRING)
	private Gender gender;
	@Basic
	@Column(name = "address")
	private String address;
	@Basic
	@Column(name = "phone_number")
	private String phoneNumber;
	@Basic
	@Column(name = "email_address")
	private String emailAddress;

	public enum Role {
		ADMIN, TEACHER, STUDENT;

		@JsonValue
		public String toLowerCase() {
			return toString().toLowerCase();
		}
	}

	@Transient
	private Role role;

	@OneToOne
	@JsonIgnore
	@JoinColumn(name = "username", referencedColumnName = "username")
	@NotFound(action = NotFoundAction.IGNORE)
	private StudentEntity student;

	@OneToOne
	@JsonIgnore
	@JoinColumn(name = "username", referencedColumnName = "username")
	@NotFound(action = NotFoundAction.IGNORE)
	private TeacherEntity teacher;

	public UserInfoEntity(
			String username,
			String securityQuestion, String securityAnswer,
			String firstName, String lastName, Gender gender,
			String address, String phoneNumber, String emailAddress,
			Role role
	) {
		this.username = username;
		this.securityQuestion = securityQuestion;
		this.securityAnswer = securityAnswer;
		this.firstName = firstName;
		this.lastName = lastName;
		this.gender = gender;
		this.address = address;
		this.phoneNumber = phoneNumber;
		this.emailAddress = emailAddress;
		this.role = role;
	}
}
