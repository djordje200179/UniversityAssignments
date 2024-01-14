package com.djordjemilanovic.backend.models;

import jakarta.persistence.*;
import lombok.*;

import java.io.Serializable;

@Setter
@Getter
@Entity
@EqualsAndHashCode
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
		private String subject;

		@ManyToOne
		@JoinColumn(name = "username", referencedColumnName = "username", nullable = false)
		private TeacherEntity teacher;
	}

	@Id
	private TeacherSubjectID id;

	public TeacherSubjectEntity(TeacherEntity teacher, String subject) {
		this.id = new TeacherSubjectID(subject, teacher);
	}
}
