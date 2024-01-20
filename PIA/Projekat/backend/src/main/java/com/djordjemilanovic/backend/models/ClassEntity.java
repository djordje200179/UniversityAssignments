package com.djordjemilanovic.backend.models;

import jakarta.persistence.*;
import lombok.*;

import java.sql.Timestamp;
import java.util.Date;

@Setter
@Getter
@EqualsAndHashCode
@Entity
@NoArgsConstructor
@Table(name = "classes", schema = "pia")
public class ClassEntity {
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Column(name = "id")
	private int id;

	@Basic
	@Column(name = "teacher")
	private String teacherUsername;

	@Basic
	@Column(name = "student")
	private String studentUsername;

	@Basic
	@Column(name = "subject")
	private String subject;

	@Basic
	@Column(name = "topic")
	private String topic;

	@Basic
	@Column(name = "time")
	private Timestamp time;

	@Basic
	@Column(name = "is_double")
	private boolean isDouble;

	@ManyToOne
	@JoinColumn(name = "teacher", referencedColumnName = "username", insertable = false, updatable = false)
	private TeacherEntity teacher;

	@ManyToOne
	@JoinColumn(name = "student", referencedColumnName = "username", insertable = false, updatable = false)
	private StudentEntity student;

	public ClassEntity(String teacher, String student, String subject, String topic, Timestamp time, boolean isDouble) {
		this.teacherUsername = teacher;
		this.studentUsername = student;
		this.subject = subject;
		this.topic = topic;
		this.time = time;
		this.isDouble = isDouble;
	}
}
