package com.djordjemilanovic.backend.models;

import jakarta.persistence.*;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
@Entity
@EqualsAndHashCode
@Table(name = "users", schema = "pia")
public class UserEntity {
	@Id
	@Column(name = "username")
	private String username;
	@Basic
	@Column(name = "password_hash")
	private String passwordHash;

	public UserEntity() {
	}

	public UserEntity(String username, String passwordHash) {
		this.username = username;
		this.passwordHash = passwordHash;
	}
}