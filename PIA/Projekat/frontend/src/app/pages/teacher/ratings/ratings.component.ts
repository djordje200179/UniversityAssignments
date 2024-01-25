import {Component, Input, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {MatCardModule} from "@angular/material/card";
import {MatDividerModule} from "@angular/material/divider";
import {Rating, TeachersService} from "../../../services/teachers.service";

@Component({
	selector: "app-ratings",
	standalone: true,
	imports: [CommonModule, MatCardModule, MatDividerModule],
	templateUrl: "./ratings.component.html",
	styleUrls: ["./ratings.component.scss"]
})
export class RatingsComponent implements OnInit {
	@Input()
	public teacherUsername!: string;

	public ratings?: Rating[];

	public constructor(private readonly teachersService: TeachersService) {

	}

	public ngOnInit(): void {
		this.teachersService.getTeacherRatings(this.teacherUsername).subscribe(
			ratings => {
				this.ratings = ratings;
			},
			console.error
		);
	}
}
