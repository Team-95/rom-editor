
import { Injectable } from '@angular/core';
import { ROM } from '../model/rom';
import addon from '../addon';

@Injectable()
export class RomService {

    rom: ROM;

    constructor() {
       
    }

    LoadROM(path: String): void{
        this.rom = new ROM();

        let test = addon.GetTeams(path);
        console.log(test);
    }


}