import {Component} from "@angular/core";
import {CommonModule} from "@angular/common";
import {FormsModule} from "@angular/forms";
import {MatButtonModule} from "@angular/material/button";
import {MatCardModule} from "@angular/material/card";
import {MatFormFieldModule} from "@angular/material/form-field";
import {MatIconModule} from "@angular/material/icon";
import {MatInputModule} from "@angular/material/input";
import {StudentInfo, TeacherInfo, UserInfo, UsersService} from "../../../../services/users.service";

@Component({
	selector: "app-info",
	standalone: true,
	imports: [CommonModule, FormsModule, MatButtonModule, MatCardModule, MatFormFieldModule, MatIconModule, MatInputModule],
	templateUrl: "./info.component.html",
	styleUrls: ["./info.component.scss"]
})
export class InfoComponent {
	public userInfo: UserInfo;
	public studentInfo: StudentInfo;
	public teacherInfo: TeacherInfo;

	public constructor(private readonly usersService: UsersService) {
		this.userInfo = usersService.getCurrentUser()!;
		this.studentInfo = usersService.getCurrentUserStudentInfo() ?? {} as StudentInfo;
		this.teacherInfo = usersService.getCurrentUserTeacherInfo() ?? {} as TeacherInfo;
	}
}
