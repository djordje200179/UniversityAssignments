import {Component, EventEmitter, OnInit, Output} from "@angular/core";
import {MatInputModule} from "@angular/material/input";
import {MatIconModule} from "@angular/material/icon";
import {MatButtonModule} from "@angular/material/button";
import {FormsModule} from "@angular/forms";
import {
	Credentials,
	Role,
	SchoolType,
	StudentInfo,
	TeacherInfo,
	UserInfo,
	UsersService
} from "../../../../services/users.service";
import {MatSelectModule} from "@angular/material/select";
import {MatButtonToggleModule} from "@angular/material/button-toggle";
import {NgForOf, NgSwitch, NgSwitchCase} from "@angular/common";
import {MatCheckboxModule} from "@angular/material/checkbox";
import {TeachersService} from "../../../../services/teachers.service";

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
export class SignUpFormComponent implements OnInit {
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

	public possibleSubjects? : string[];

	public constructor(private readonly teachersService: TeachersService, private readonly usersService: UsersService) {

	}

	public ngOnInit() {
		this.teachersService.getAllSubjects().subscribe(
			subjects => {
				this.possibleSubjects = subjects.map(s => s.name);
			},
			console.error
		);
	}

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
		if (this.credentials.username === "")
			return alert("Нисте унијели корисничко име.");

		const passwordError = this.usersService.isPasswordValid(this.credentials.password);
		if (passwordError)
			return alert(passwordError);

		if (this.userInfo.securityQuestion === "")
			return alert("Нисте унијели сигурносно питање.");

		if (this.userInfo.securityAnswer === "")
			return alert("Нисте унијели сигурносни одговор.");

		if (this.userInfo.firstName === "")
			return alert("Нисте унијели име.");

		if (this.userInfo.lastName === "")
			return alert("Нисте унијели презиме.");

		if (this.userInfo.gender === null)
			return alert("Нисте одабрали пол.");

		if (this.userInfo.address === "")
			return alert("Нисте унијели адресу.");

		if (this.userInfo.emailAddress === "")
			return alert("Нисте унијели и-мејл адресу.");

		if (this.userInfo.phoneNumber === "")
			return alert("Нисте унијели број телефона.");

		if (this.userInfo.role === Role.Student) {
			if (this.studentInfo.schoolType === null)
				return alert("Нисте одабрали тип школе.");

			if (this.studentInfo.schoolYear === null)
				return alert("Нисте одабрали разред.");
		} else {
			if (this.teacherInfo.biography === null)
				return alert("Нисте одабрали биографију.");

			if (this.teacherInfo.subjects.length === 0)
				return alert("Нисте одабрали предмете.");

			if (this.ageGroups.length === 0)
				return alert("Нисте одабрали узрастне групе.");
		}

		this.teacherInfo.teachesLowerElementary = this.ageGroups.includes("lowerElementary");
		this.teacherInfo.teachesUpperElementary = this.ageGroups.includes("upperElementary");
		this.teacherInfo.teachesHigh = this.ageGroups.includes("high");

		if (this.userInfo.role === Role.Student)
			this.signUpStudent.emit(this.studentInfo);
		else
			this.signUpTeacher.emit(this.teacherInfo);
	}
}
