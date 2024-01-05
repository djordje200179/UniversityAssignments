package com.djordjemilanovic.backend.models;

import jakarta.persistence.*;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
@Entity
@EqualsAndHashCode
@Table(name = "users_info", schema = "pia")
public class UserInfoEntity {
	@GeneratedValue(strategy = GenerationType.IDENTITY)
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
	private Object gender;
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
