import {Component, Input} from "@angular/core";
import {CommonModule} from "@angular/common";
import {FormsModule} from "@angular/forms";
import {MatButtonModule} from "@angular/material/button";
import {MatCardModule} from "@angular/material/card";
import {MatFormFieldModule} from "@angular/material/form-field";
import {MatIconModule} from "@angular/material/icon";
import {MatInputModule} from "@angular/material/input";
import {SchoolType, StudentInfo, TeacherInfo, UserInfo, UsersService} from "../../../../services/users.service";
import {HttpErrorResponse} from "@angular/common/http";

@Component({
	selector: "app-info",
	standalone: true,
	imports: [CommonModule, FormsModule, MatButtonModule, MatCardModule, MatFormFieldModule, MatIconModule, MatInputModule],
	templateUrl: "./info.component.html",
	styleUrls: ["./info.component.scss"]
})
export class InfoComponent {
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

	public constructor(private readonly usersService: UsersService) {
		this.userInfo = usersService.getCurrentUser()!;
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
}
