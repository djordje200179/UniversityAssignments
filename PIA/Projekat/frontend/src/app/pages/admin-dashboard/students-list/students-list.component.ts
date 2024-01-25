import {Component} from "@angular/core";
import {CommonModule} from "@angular/common";
import {StudentInfo, TeacherInfo, UsersService} from "../../../services/users.service";
import {MatButtonModule} from "@angular/material/button";
import {MatCardModule} from "@angular/material/card";
import {MatDividerModule} from "@angular/material/divider";

@Component({
	selector: "app-students-list",
	standalone: true,
	imports: [CommonModule, MatButtonModule, MatCardModule, MatDividerModule],
	templateUrl: "./students-list.component.html",
	styleUrls: ["./students-list.component.scss"]
})
export class StudentsListComponent {
	public students?: StudentInfo[];

	public constructor(public readonly usersService: UsersService) {
	}

	public ngOnInit(): void {
		this.usersService.getAllStudents().subscribe(
			students => this.students = students,
			console.error
		);
	}
}
