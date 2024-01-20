import {Component, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {UsersService} from "../../../services/users.service";
import {ClassesService} from "../../../services/classes.service";
import {MatSortModule} from "@angular/material/sort";
import {MatTableModule} from "@angular/material/table";
import {MatButtonModule} from "@angular/material/button";

interface Row {
	teacherName: string;
	topic: string;
	subject: string;
	teacherComment?: string;
	studentComment?: string;
	time: Date;
}

@Component({
	selector: "app-student-archive",
	standalone: true,
	imports: [CommonModule, MatSortModule, MatTableModule, MatButtonModule],
	templateUrl: "./student-archive.component.html",
	styleUrls: ["./student-archive.component.scss"]
})
export class StudentArchiveComponent implements OnInit {
	public tableData?: Row[];

	public displayedColumns: string[] = ["topic", "subject", "teacherName", "time", "teacherComment", "studentComment"];


	public constructor(private readonly usersService: UsersService, private readonly classesService: ClassesService) {

	}

	public ngOnInit(): void {
		const studentUsername = this.usersService.getCurrentUser()!.username!;

		this.classesService.getArchive(studentUsername).subscribe(
			classes => {
				const tableData: Row[] = [];
				for (const c of classes) {
					c.time = new Date(c.time);

					const teacherInfo = c.teacher.info;
					const teacherName = `${teacherInfo.firstName} ${teacherInfo.lastName}`;

					tableData.push({
						teacherName: teacherName,
						topic: c.topic,
						subject: c.subject,
						teacherComment: c.teacherComment || undefined,
						studentComment: c.studentComment || undefined,
						time: c.time
					});
				}

				tableData.sort((a, b) => b.time.getTime() - a.time.getTime());

				this.tableData = tableData;
			},
			console.error
		);
	}
}
