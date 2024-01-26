import {Component, Input} from "@angular/core";
import {CommonModule} from "@angular/common";
import {FormsModule} from "@angular/forms";
import {MatButtonModule} from "@angular/material/button";
import {MatCardModule} from "@angular/material/card";
import {MatFormFieldModule} from "@angular/material/form-field";
import {MatIconModule} from "@angular/material/icon";
import {MatInputModule} from "@angular/material/input";
import {SchoolType, StudentInfo, TeacherInfo, UserInfo, UsersService} from "../../../../services/users.service";

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
				window.location.reload();
			},
			console.error
		);
	}
}
