import {Component, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {TeacherInfo, UsersService} from "../../../services/users.service";
import {MatButtonModule} from "@angular/material/button";
import {MatCardModule} from "@angular/material/card";
import {MatDividerModule} from "@angular/material/divider";
import {TeachersService} from "../../../services/teachers.service";

@Component({
	selector: "app-teachers-list",
	standalone: true,
	imports: [CommonModule, MatButtonModule, MatCardModule, MatDividerModule],
	templateUrl: "./teachers-list.component.html",
	styleUrls: ["./teachers-list.component.scss"]
})
export class TeachersListComponent implements OnInit {
	public teachers?: TeacherInfo[];

	public constructor(public readonly usersService: UsersService, public readonly teachersService: TeachersService) {
	}

	public ngOnInit(): void {
		this.usersService.getAllTeachers().subscribe(
			teachers => {
				for (const teacher of teachers) {
					this.teachersService.getTeacherEnrollments(teacher.info.username!).subscribe(
						enrollments => {
							teacher.subjects = enrollments.map(enrollment => enrollment.subject.name)
						},
						console.error
					)
				}

				this.teachers = teachers;
			},
			console.error
		);
	}

	public blockTeacher(teacher: TeacherInfo) {
		this.teachersService.blockTeacher(teacher.info.username!).subscribe(
			() => {
				teacher.activated = false;
				teacher.blocked = true;
			},
			console.error
		);
	}
}
