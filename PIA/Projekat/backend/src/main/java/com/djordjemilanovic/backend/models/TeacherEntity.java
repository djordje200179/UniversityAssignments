package com.djordjemilanovic.backend.models;

import jakarta.persistence.*;
import lombok.*;

import java.util.Set;

@Setter
@Getter
@Entity
@EqualsAndHashCode
@NoArgsConstructor
@AllArgsConstructor
@Table(name = "teachers", schema = "pia")
public class TeacherEntity {
	@Id
	@Column(name = "username")
	private String username;

	@OneToMany
	@JoinColumn(name = "username", referencedColumnName = "username")
	private Set<TeacherSubjectEntity> subjects;

	@OneToOne
	@JoinColumn(name = "username", referencedColumnName = "username")
	private UserInfoEntity userInfo;

	public TeacherEntity(String username) {
		this.username = username;
	}
}
