import { Component } from '@angular/core';
import { ElectronService } from './providers/electron.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  constructor(public electronService: ElectronService) {

    if (electronService.isElectron()) {
      console.log('Mode electron');
      // Check if electron is correctly injected (see externals in webpack.config.js)
      console.log('c', electronService.ipcRenderer);
      // Check if nodeJs childProcess is correctly injected (see externals in webpack.config.js)
      console.log('c', electronService.childProcess);
    } else {
      console.log('Mode web');
    }
  }
}

// Custom frame
(function () {
      
    const remote = require('electron').remote; 
    
    function init() { 
      document.getElementById("min-btn").addEventListener("click", function (e) {
        const window = remote.getCurrentWindow();
        window.minimize(); 
      });
      
      document.getElementById("max-btn").addEventListener("click", function (e) {
        const window = remote.getCurrentWindow();
        if (!window.isMaximized()) {
          window.maximize();
        } else {
          window.unmaximize();
        }	 
      });
      
      document.getElementById("close-btn").addEventListener("click", function (e) {
        const window = remote.getCurrentWindow();
        window.close();
      }); 
    }; 
    
    document.onreadystatechange = function () {
      if (document.readyState == "complete") {
        init(); 
      }
    };
})();
