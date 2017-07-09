import { Directive, HostListener, ElementRef, Input } from '@angular/core';

@Directive({
  selector: '[appInputFormat]'
})
export class InputFormatDirective {

  @Input('appInputFormat') format: string;

  constructor(private element: ElementRef) { }

  @HostListener('focus')
  onFocus(){
    console.log('focused');
  }

  @HostListener('blur')
  onBlur(){
    let value: string = this.element.nativeElement.value;
    if(this.format == 'lowercase'){
      this.element.nativeElement.value = value.toLocaleLowerCase();
    }
    else{
      this.element.nativeElement.value = value.toLocaleUpperCase();
    }
  }

}
