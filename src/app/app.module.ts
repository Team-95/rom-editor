import 'zone.js';
import 'reflect-metadata';
import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';

import { AppComponent } from './app.component';
import { HomeComponent } from './components/home/home.component';
import { MessageComponent} from './components/message/message.component';

import { AppRoutingModule } from './app-routing.module';

import { ElectronService } from './providers/electron.service';
import { InputFormatDirective } from './input-format.directive';
import { FileComponent } from './components/file/file.component';
import { StatsComponent } from './components/stats/stats.component';
import { RomService } from './providers/rom.service';

@NgModule({
  declarations: [
    AppComponent,
    HomeComponent,
    MessageComponent,
    InputFormatDirective,
    FileComponent,
    StatsComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpModule,
    AppRoutingModule
  ],
  providers: [
    ElectronService,
    RomService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
