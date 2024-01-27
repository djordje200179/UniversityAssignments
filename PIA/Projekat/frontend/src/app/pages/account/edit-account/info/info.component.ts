import {Component, Input, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {FormsModule} from "@angular/forms";
import {MatButtonModule} from "@angular/material/button";
import {MatCardModule} from "@angular/material/card";
import {MatFormFieldModule} from "@angular/material/form-field";
import {MatIconModule} from "@angular/material/icon";
import {MatInputModule} from "@angular/material/input";
import {SchoolType, StudentInfo, TeacherInfo, UserInfo, UsersService} from "../../../../services/users.service";
import {HttpErrorResponse} from "@angular/common/http";
import {MatOptionModule} from "@angular/material/core";
import {MatSelectModule} from "@angular/material/select";
import {TeachersService} from "../../../../services/teachers.service";
import {MatCheckboxModule} from "@angular/material/checkbox";

@Component({
	selector: "app-info",
	standalone: true,
	imports: [CommonModule, FormsModule, MatButtonModule, MatCardModule, MatFormFieldModule, MatIconModule, MatInputModule, MatOptionModule, MatSelectModule, MatCheckboxModule],
	templateUrl: "./info.component.html",
	styleUrls: ["./info.component.scss"]
})
export class InfoComponent implements OnInit {
	public userInfo: UserInfo;

	public readonly schoolNames : {[key in SchoolType]: string} = {
		[SchoolType.Elementary]: "Основна школа",
		[SchoolType.Gymnasium]: "Гимназија",
		[SchoolType.Specialized]: "Стручна школа",
		[SchoolType.Art]: "Умјетничка школа"
	}

	@Input()
	public studentInfo?: StudentInfo;

	@Input()
	public teacherInfo?: TeacherInfo;

	public possibleSubjects? : string[];
	public teacherSubjects? : string[];

	public constructor(private readonly usersService: UsersService, private readonly teachersService: TeachersService, ) {
		this.userInfo = usersService.getCurrentUser()!;
	}

	public ngOnInit() {
		this.teachersService.getAllSubjects().subscribe(
			subjects => {
				this.possibleSubjects = subjects.map(s => s.name);
			},
			console.error
		);

		this.teachersService.getTeacherEnrollments(this.userInfo.username!).subscribe(
			enrollments => {
				this.teacherSubjects = enrollments.map(e => e.subject.name);
			},
			console.error
		);

	}

	public incrementYear() {
		this.usersService.incrementSchoolYear(this.userInfo.username!).subscribe(
			student => {
				Object.assign(this.studentInfo!, student);
			},
			console.error
		);
	}

	public updateProfile() {
		this.usersService.updateUser(this.userInfo).subscribe(
			user => {
				localStorage.setItem("user-info", JSON.stringify(user));
				this.userInfo = user;
				alert("Профил је успјешно ажуриран");
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

	public updateTeacherInfo() {
		this.teachersService.updateTeacherInfo(
			this.userInfo.username!, this.teacherSubjects!,
			this.teacherInfo!.teachesLowerElementary, this.teacherInfo!.teachesUpperElementary,
			this.teacherInfo!.teachesHigh
		).subscribe(
			data => {
				Object.assign(this.teacherInfo!, data);
				alert("Профил је успјешно ажуриран");
			},
			console.error
		);
	}
}
