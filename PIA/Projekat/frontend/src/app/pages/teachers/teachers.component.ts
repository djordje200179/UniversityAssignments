import {Component, OnInit, ViewChild} from "@angular/core";
import {CommonModule} from "@angular/common";
import {MatSort, MatSortModule} from "@angular/material/sort";
import {MatTableDataSource, MatTableModule} from "@angular/material/table";
import {TeachersService} from "../../services/teachers.service";
import {SchoolType, StudentInfo, UsersService} from "../../services/users.service";
import {MatIconModule} from "@angular/material/icon";
import {MatButtonModule} from "@angular/material/button";
import {Router} from "@angular/router";
import {MatInputModule} from "@angular/material/input";
import {FormsModule, ReactiveFormsModule} from "@angular/forms";

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
	imports: [CommonModule, MatSortModule, MatTableModule, MatIconModule, MatButtonModule, MatInputModule, ReactiveFormsModule, FormsModule],
	templateUrl: "./teachers.component.html",
	styleUrls: ["./teachers.component.scss"]
})
export class TeachersComponent implements OnInit {
	public allData? : Row[];

	public searchedSubject = "";
	public searchedFirstName = "";
	public searchedLastName = "";

	public displayedColumns: string[] = ["firstName", "lastName", "subject", "rating"];

	public studentInfo: StudentInfo;

	public constructor(
		private readonly teachersService: TeachersService, private readonly usersService: UsersService,
		private readonly router: Router) {
		this.studentInfo = usersService.getCurrentUserStudentInfo() ?? {} as StudentInfo;
	}

	@ViewChild(MatSort)
	public sort?: MatSort;

	public getTableData() : MatTableDataSource<Row> {
		const filteredData = this.allData?.filter(row => {
			return row.subject.toLowerCase().includes(this.searchedSubject.toLowerCase()) &&
				row.firstName.toLowerCase().includes(this.searchedFirstName.toLowerCase()) &&
				row.lastName.toLowerCase().includes(this.searchedLastName.toLowerCase());
		});

		const source = new MatTableDataSource(filteredData);
		source.sort = this.sort!;

		return source;
	}

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
					rating: 0,
				}));

				for (const row of tableData) {
					this.teachersService.getTeacherRatings(row.username).subscribe(
						ratings => {
							const sum = ratings.reduce((a, b) => a + b.rating, 0);
							const average = ratings.length != 0 ? sum / ratings.length : 0;
							row.rating = Math.floor(average);
						},
						console.error
					);
				}

				this.allData = tableData;
			},
			console.error
		);
	}

	public rowClicked(row: Row) {
		this.router.navigate(["/teacher", row.username]);
	}
}
