import {Component, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {TeacherInfo, UsersService} from "../../../services/users.service";
import {MatCardModule} from "@angular/material/card";
import {MatButtonModule} from "@angular/material/button";
import {MatDividerModule} from "@angular/material/divider";
import {TeachersService} from "../../../services/teachers.service";

@Component({
	selector: "app-teacher-requests",
	standalone: true,
	imports: [CommonModule, MatCardModule, MatButtonModule, MatDividerModule],
	templateUrl: "./teacher-requests.component.html",
	styleUrls: ["./teacher-requests.component.scss"]
})
export class TeacherRequestsComponent implements OnInit {
	public teachers?: TeacherInfo[];

	public constructor(public readonly usersService: UsersService, public readonly teachersService: TeachersService) {
	}

	public ngOnInit(): void {
		this.usersService.getAllTeacherRequests().subscribe(
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

	public blockTeacher(username: string) {
		this.teachersService.blockTeacher(username).subscribe(
			() => {
				this.teachers = this.teachers?.filter(teacher => teacher.info.username !== username);
			},
			console.error
		);
	}

	public acceptTeacher(username: string) {
		this.teachersService.acceptTeacher(username).subscribe(
			() => {
				this.teachers = this.teachers?.filter(teacher => teacher.info.username !== username);
			},
			console.error
		);
	}
}
