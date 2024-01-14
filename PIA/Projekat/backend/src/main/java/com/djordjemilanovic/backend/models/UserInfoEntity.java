package com.djordjemilanovic.backend.models;

import jakarta.persistence.*;
import lombok.*;
import org.hibernate.annotations.ColumnTransformer;

import java.io.Serializable;

@Setter
@Getter
@Entity
@EqualsAndHashCode
@NoArgsConstructor
@AllArgsConstructor
@Table(name = "users_info", schema = "pia")
public class UserInfoEntity {
	public enum Gender {
		FEMALE, MALE
	}

	@Id
	@Column(name = "username")
	private String username;
	@Basic
	@Column(name = "security_question")
	private String securityQuestion;
	@Basic
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
}
