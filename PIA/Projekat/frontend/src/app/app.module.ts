import {NgModule} from "@angular/core";
import {BrowserModule} from "@angular/platform-browser";

import {AppRoutingModule} from "./app-routing.module";
import {AppComponent} from "./app.component";
import {BrowserAnimationsModule} from "@angular/platform-browser/animations";
import {HeaderComponent} from "./components/header/header.component";
import {HttpClientModule} from "@angular/common/http";
import {AccountComponent} from "./pages/account/account.component";
import {TeachersComponent} from "./pages/teachers/teachers.component";

@NgModule({
	declarations: [
		AppComponent,
	],
	imports: [
		BrowserModule,
		AppRoutingModule,
		BrowserAnimationsModule,
		HttpClientModule,

		HeaderComponent,
		AccountComponent,
		TeachersComponent
	],
	providers: [],
	bootstrap: [AppComponent]
})
export class AppModule {
}
