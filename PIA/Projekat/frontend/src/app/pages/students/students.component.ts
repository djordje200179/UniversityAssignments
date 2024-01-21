import {Component, OnInit, ViewChild} from "@angular/core";
import {CommonModule} from "@angular/common";
import {MatIconModule} from "@angular/material/icon";
import {MatSort, MatSortModule} from "@angular/material/sort";
import {MatTableModule} from "@angular/material/table";
import {StudentInfo, UsersService} from "../../services/users.service";
import {TeachersService} from "../../services/teachers.service";
import {Router} from "@angular/router";

interface Row {
	username: string;

	firstName: string;
	lastName: string;
}


@Component({
	selector: "app-students",
	standalone: true,
	imports: [CommonModule, MatIconModule, MatSortModule, MatTableModule],
	templateUrl: "./students.component.html",
	styleUrls: ["./students.component.scss"]
})
export class StudentsComponent implements OnInit {
	public tableData?: Row[];

	public displayedColumns: string[] = ["firstName", "lastName"];

	public studentInfo: StudentInfo;

	public constructor(
		private readonly teachersService: TeachersService, private readonly usersService: UsersService,
		private readonly router: Router) {
		this.studentInfo = usersService.getCurrentUserStudentInfo() ?? {} as StudentInfo;
	}

	@ViewChild(MatSort)
	public sort?: MatSort;

	public ngOnInit() {
		this.teachersService.getTeacherStudents(this.usersService.getCurrentUser()!.username!).subscribe(
			students => {
				const tableData = students.map(student => ({
					username: student.info.username!,
					firstName: student.info.firstName,
					lastName: student.info.lastName,
				}));

				this.tableData = tableData;
			},
			console.error
		);
	}

	public rowClicked(row: Row) {
		this.router.navigate(["/student", row.username]);
	}
}
