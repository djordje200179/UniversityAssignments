import {Component, OnInit, ViewChild} from "@angular/core";
import {CommonModule} from "@angular/common";
import {MatSort, MatSortModule} from "@angular/material/sort";
import {MatTableDataSource, MatTableModule} from "@angular/material/table";
import {TeachersService} from "../../services/teachers.service";
import {SchoolType, StudentInfo, UsersService} from "../../services/users.service";
import {MatIconModule} from "@angular/material/icon";
import {MatButtonModule} from "@angular/material/button";
import {Router} from "@angular/router";

interface Row {
	username: string;

	firstName: string;
	lastName: string;
	subject: string;
	rating: number;
}

@Component({
	selector: "app-teachers",
	standalone: true,
	imports: [CommonModule, MatSortModule, MatTableModule, MatIconModule, MatButtonModule],
	templateUrl: "./teachers.component.html",
	styleUrls: ["./teachers.component.scss"]
})
export class TeachersComponent implements OnInit {
	public tableData?: MatTableDataSource<Row>;

	public displayedColumns: string[] = ["firstName", "lastName", "subject", "rating"];

	public studentInfo: StudentInfo;

	public constructor(
		private readonly teachersService: TeachersService, private readonly usersService: UsersService,
		private readonly router: Router) {
		this.studentInfo = usersService.getCurrentUserStudentInfo() ?? {} as StudentInfo;
	}

	@ViewChild(MatSort)
	public sort?: MatSort;

	public ngOnInit() {
		this.teachersService.getAllEnrollments(
			this.studentInfo.schoolType == SchoolType.Elementary, this.studentInfo.schoolYear
		).subscribe(
			enrollments => {
				const tableData = enrollments.map(enrollment => ({
					username: enrollment.teacher.info.username!,
					firstName: enrollment.teacher.info.firstName,
					lastName: enrollment.teacher.info.lastName,
					subject: enrollment.subject.name,
					rating: 3
				}));

				const source = new MatTableDataSource(tableData);
				source.sort = this.sort!;

				this.tableData = source;
			},
			console.error
		);
	}

	public rowClicked(row: Row) {
		this.router.navigate(["/teacher", row.username]);
	}
}
