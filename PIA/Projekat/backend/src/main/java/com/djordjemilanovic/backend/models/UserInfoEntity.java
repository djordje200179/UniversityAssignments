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
	public enum Gender {
		FEMALE, MALE;


		public static class Converter implements AttributeConverter<Gender, String> {
			@Override
			public String convertToDatabaseColumn(Gender gender) {
				return gender.name().toLowerCase();
			}

			@Override
			public Gender convertToEntityAttribute(String s) {
				return Gender.valueOf(s.toUpperCase());
			}
		}
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
	@Enumerated(EnumType.STRING)
	@Convert(converter = Gender.Converter.class)
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

	public UserInfoEntity() {
	}

	public UserInfoEntity(
			String username,
			String securityQuestion,
			String securityAnswer,
			String firstName, String lastName, Gender gender,
			String address, String phoneNumber, String emailAddress
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
	}
}
