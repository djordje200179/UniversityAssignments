package com.djordjemilanovic.backend.models;

import jakarta.persistence.*;
import lombok.*;

@Setter
@Getter
@Entity
@EqualsAndHashCode
@NoArgsConstructor
@AllArgsConstructor
@Table(name = "users", schema = "pia")
public class UserEntity {
	@Id
	@Column(name = "username")
	private String username;
	@Basic
	@Column(name = "password_hash")
	private String passwordHash;
}
