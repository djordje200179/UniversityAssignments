package com.djordjemilanovic.backend.models;

import jakarta.persistence.*;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
@EqualsAndHashCode
@Entity
@Table(name = "students", schema = "pia")
public class StudentEntity {
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Id
	@Column(name = "username")
	private String username;
	@Basic
	@Column(name = "school_type")
	private Object schoolType;
	@Basic
	@Column(name = "school_year")
	private int schoolYear;
}
