import {NgModule} from "@angular/core";
import {BrowserModule} from "@angular/platform-browser";

import {AppRoutingModule} from "./app-routing.module";
import {AppComponent} from "./app.component";
import {BrowserAnimationsModule} from "@angular/platform-browser/animations";
import {HeaderComponent} from "./components/header/header.component";
import {WelcomeComponent} from "./pages/welcome/welcome.component";
import {HttpClientModule} from "@angular/common/http";

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
		WelcomeComponent
	],
	providers: [],
	exports: [
	],
	bootstrap: [AppComponent]
})
export class AppModule {
}
