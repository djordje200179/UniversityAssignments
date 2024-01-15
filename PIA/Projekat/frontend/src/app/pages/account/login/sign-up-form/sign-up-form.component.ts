import {Component, EventEmitter, Output} from "@angular/core";
import {MatInputModule} from "@angular/material/input";
import {MatIconModule} from "@angular/material/icon";
import {MatButtonModule} from "@angular/material/button";
import {FormsModule} from "@angular/forms";
import {Credentials, Role, SchoolType, StudentInfo, TeacherInfo, UserInfo} from "../../../../services/users.service";
import {MatSelectModule} from "@angular/material/select";
import {MatButtonToggleModule} from "@angular/material/button-toggle";
import {NgForOf, NgSwitch, NgSwitchCase} from "@angular/common";
import {MatCheckboxModule} from "@angular/material/checkbox";

@Component({
	selector: "app-sign-up-form",
	templateUrl: "./sign-up-form.component.html",
	styleUrls: ["./sign-up-form.component.scss"],
	standalone: true,
	imports: [
		MatInputModule,
		MatIconModule,
		MatButtonModule,
		FormsModule,
		MatSelectModule,
		MatButtonToggleModule,
		NgSwitch,
		NgSwitchCase,
		NgForOf,
		MatCheckboxModule
	]
})
export class SignUpFormComponent {
	public credentials: Credentials = {
		username: "",
		password: ""
	};

	public userInfo: UserInfo = {
		role: Role.Student,

		firstName: "",
		lastName: "",

		securityAnswer: "",
		securityQuestion: "",

		address: "",
		emailAddress: "",
		phoneNumber: "",
		gender: null!
	};

	public studentInfo: StudentInfo = {
		credentials: this.credentials,
		info: this.userInfo,

		schoolType: null!,
		schoolYear: null!
	}

	public get schoolYears(): number[] {
		switch (this.studentInfo.schoolType) {
		case SchoolType.Elementary:
			return [1, 2, 3, 4, 5, 6, 7, 8];
		case SchoolType.Gymnasium:
		case SchoolType.Specialized:
		case SchoolType.Art:
			return [1, 2, 3, 4];
		default:
			return [];
		}
	}

	public teacherInfo: TeacherInfo = {
		credentials: this.credentials,
		info: this.userInfo,

		biography: null!,
		subjects: [] as string[],

		teachesLowerElementary: false,
		teachesUpperElementary: false,
		teachesHigh: false
	}

	public ageGroups: string[] = [];

	public readonly possibleSubjects = [
		"Математика", "Физика", "Хемија", "Информатика", "Програмирање",
		"Српски језик и књижевност", "Енглески језик", "Немачки језик", "Италијански језик",
		"Француски језик", "Шпански језик", "Латински језик", "Биологија", "Историја",
		"Географија", "Свет око нас"
	];

	@Output()
	public readonly signUpStudent = new EventEmitter<StudentInfo>();

	@Output()
	public readonly signUpTeacher = new EventEmitter<TeacherInfo>();

	public onBiographySelected(event: Event) {
		const target = event.target as HTMLInputElement;
		const files = target?.files;
		if (!files || files.length === 0)
			return;

		this.teacherInfo.biography = files[0];
	}

	public onProfileImageSelected(event: Event) {
		const target = event.target as HTMLInputElement;
		const files = target?.files;
		if (!files || files.length === 0)
			return;

		this.userInfo.profileImage = files[0];
	}

	public onSignUp() {
		this.teacherInfo.teachesLowerElementary = this.ageGroups.includes("lowerElementary");
		this.teacherInfo.teachesUpperElementary = this.ageGroups.includes("upperElementary");
		this.teacherInfo.teachesHigh = this.ageGroups.includes("high");

		if (this.userInfo.role === Role.Student)
			this.signUpStudent.emit(this.studentInfo);
		else
			this.signUpTeacher.emit(this.teacherInfo);
	}
}
