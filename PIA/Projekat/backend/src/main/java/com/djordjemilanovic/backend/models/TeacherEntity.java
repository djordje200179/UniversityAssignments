package com.djordjemilanovic.backend.models;

import jakarta.persistence.*;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
@Entity
@EqualsAndHashCode
@Table(name = "teachers", schema = "pia")
public class TeacherEntity {
	@Id
	@Column(name = "username")
	private String username;

	public TeacherEntity() {
	}

	public TeacherEntity(String username) {
		this.username = username;
	}
}
