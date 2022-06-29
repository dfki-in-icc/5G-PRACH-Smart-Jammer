import { Component } from '@angular/core';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css'],
})
export class AppComponent {
  title = 'oai softmodem';
  isscopeavailable = false;
  scopelabel = '';
  constructor() {
    this.scopelabel = 'Scope';
    this.isscopeavailable=true;
  }
  
  onScopeDisabled( disabled: boolean) {
    this.isscopeavailable= !disabled;
  }
}
