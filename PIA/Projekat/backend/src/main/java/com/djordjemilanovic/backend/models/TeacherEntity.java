package com.djordjemilanovic.backend.models;

import com.fasterxml.jackson.annotation.JsonIgnore;
import jakarta.persistence.*;
import lombok.*;

import java.util.Set;

@Setter
@Getter
@Entity
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
@NoArgsConstructor
@Table(name = "teachers", schema = "pia")
public class TeacherEntity {
	@Id
	@Column(name = "username")
	@EqualsAndHashCode.Include
	private String username;

	@Basic
	@Column(name = "teaches-lower-elementary")
	private boolean teachesLowerElementary;

	@Basic
	@Column(name = "teaches-upper-elementary")
	private boolean teachesUpperElementary;

	@Basic
	@Column(name = "teaches-high")
	private boolean teachesHigh;

	@Basic
	@Column(name = "activated")
	private boolean activated;

	@Basic
	@Column(name = "blocked")
	private boolean blocked;


	@OneToMany
	@JoinColumn(name = "username", referencedColumnName = "username")
	@JsonIgnore
	private Set<TeacherSubjectEntity> subjects;

	@OneToOne
	@JoinColumn(name = "username", referencedColumnName = "username")
	private UserInfoEntity info;

	public TeacherEntity(
			String username,
			boolean teachesLowerElementary, boolean teachesUpperElementary, boolean teachesHigh
	) {
		this.username = username;
		this.teachesLowerElementary = teachesLowerElementary;
		this.teachesUpperElementary = teachesUpperElementary;
		this.teachesHigh = teachesHigh;
		this.activated = false;
		this.blocked = false;
	}

	@OneToMany
	@JoinColumn(name = "teacher", referencedColumnName = "username")
	@JsonIgnore
	private Set<ClassEntity> classes;
}
