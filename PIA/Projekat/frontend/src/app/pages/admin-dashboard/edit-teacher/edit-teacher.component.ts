import {Component, Inject, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {MAT_DIALOG_DATA, MatDialogModule, MatDialogRef} from "@angular/material/dialog";
import {TeacherInfo, UsersService} from "../../../services/users.service";
import {MatButtonModule} from "@angular/material/button";
import {MatCheckboxModule} from "@angular/material/checkbox";
import {MatFormFieldModule} from "@angular/material/form-field";
import {MatIconModule} from "@angular/material/icon";
import {MatInputModule} from "@angular/material/input";
import {MatOptionModule} from "@angular/material/core";
import {MatSelectModule} from "@angular/material/select";
import {FormsModule, ReactiveFormsModule} from "@angular/forms";
import {TeachersService} from "../../../services/teachers.service";
import {HttpErrorResponse} from "@angular/common/http";

@Component({
	selector: "app-edit-teacher",
	standalone: true,
	imports: [CommonModule, MatButtonModule, MatCheckboxModule, MatFormFieldModule, MatIconModule, MatInputModule, MatOptionModule, MatSelectModule, ReactiveFormsModule, FormsModule, MatDialogModule],
	templateUrl: "./edit-teacher.component.html",
	styleUrls: ["./edit-teacher.component.scss"]
})
export class EditTeacherComponent implements OnInit {
	public possibleSubjects? : string[];
	public teacherSubjects? : string[];

	public constructor(
		public dialogRef: MatDialogRef<EditTeacherComponent, TeacherInfo>,
		@Inject(MAT_DIALOG_DATA) public data: TeacherInfo,
		private readonly usersService: UsersService, private readonly teachersService: TeachersService) {
		console.log(data);
	}

	public ngOnInit() {
		this.teachersService.getAllSubjects().subscribe(
			subjects => {
				this.possibleSubjects = subjects.map(s => s.name);
			},
			console.error
		);

		this.teachersService.getTeacherEnrollments(this.data.info.username!).subscribe(
			enrollments => {
				this.teacherSubjects = enrollments.map(e => e.subject.name);
			},
			console.error
		);
	}

	public updateTeacher() {
		this.usersService.updateUser(this.data.info).subscribe(
			user => {
				localStorage.setItem("user-info", JSON.stringify(user));
				this.data.info = user;

				this.teachersService.updateTeacherInfo(
					this.data.info.username!, this.teacherSubjects!,
					this.data.teachesLowerElementary, this.data.teachesUpperElementary,
					this.data.teachesHigh
				).subscribe(
					data => {
						Object.assign(this.data!, data);
						alert("Профил је успјешно ажуриран");
						this.dialogRef.close(this.data);
					},
					console.error
				);
			},
			err => {
				if (!(err instanceof HttpErrorResponse)) {
					alert("Непозната грешка");
					console.error(err);
					return;
				}

				if (err.status === 409) {
					alert("И-мејл адреса је већ у употреби");
					return;
				}

				alert("Непозната грешка");
				console.error(err);
				return;
			}
		);
	}
}