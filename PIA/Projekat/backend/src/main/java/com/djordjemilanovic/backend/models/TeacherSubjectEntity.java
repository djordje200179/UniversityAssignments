package com.djordjemilanovic.backend.models;

import com.fasterxml.jackson.annotation.JsonUnwrapped;
import jakarta.persistence.*;
import lombok.*;

import java.io.Serializable;

@Setter
@Getter
@Entity
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
@NoArgsConstructor
@Table(name = "teacher_subjects", schema = "pia")
public class TeacherSubjectEntity {
	@Getter
	@Setter
	@EqualsAndHashCode
	@NoArgsConstructor
	@AllArgsConstructor
	@Embeddable
	public static class TeacherSubjectID implements Serializable {
		@ManyToOne
		@JoinColumn(name = "subject", referencedColumnName = "subject", nullable = false)
		private SubjectEntity subject;

		@ManyToOne
		@JoinColumn(name = "username", referencedColumnName = "username", nullable = false)
		private TeacherEntity teacher;
	}

	@Id
	@JsonUnwrapped
	@EqualsAndHashCode.Include
	private TeacherSubjectID id;

	public TeacherSubjectEntity(TeacherEntity teacher, SubjectEntity subject) {
		this.id = new TeacherSubjectID(subject, teacher);
	}
}
