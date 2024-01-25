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
@Table(name = "subjects", schema = "pia")
public class SubjectEntity {
	@Id
	@Column(name = "subject")
	@EqualsAndHashCode.Include
	private String name;

	@OneToMany
	@JoinColumn(name = "subject", referencedColumnName = "subject")
	@JsonIgnore
	private Set<TeacherSubjectEntity> teachers;

	public SubjectEntity(String name) {
		this.name = name;
	}
}
