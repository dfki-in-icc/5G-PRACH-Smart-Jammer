import { FormControl, FormGroup } from '@angular/forms';
import { IInfo } from '../api/commands.api';
import { IArgType } from '../api/commands.api';

const enum InfosFCN {
  name = 'name',
  value = "value",
  type = "type",
  modifiable = "modifiable"
}


export class InfoCtrl extends FormGroup {
  type: IArgType;
  constructor(ivar: IInfo) {
    super({});
    this.type = ivar.type;
    this.addControl(InfosFCN.name, new FormControl(ivar.name));
    this.addControl(InfosFCN.value, new FormControl(ivar.value));
    this.addControl(InfosFCN.type, new FormControl(ivar.type));
    this.addControl(InfosFCN.modifiable, new FormControl(ivar.modifiable));
  }

  api() {
    const doc: IInfo = {
      name: this.nameFC.value,
      value: String(this.valueFC.value),  //FIXME 
      type: this.typeFC.value,
      modifiable: this.modifiableFC.value
    };

    return doc;
  }

  get nameFC() {
    return this.get(InfosFCN.name) as FormControl;
  }

  set nameFC(control: FormControl) {
    this.setControl(InfosFCN.name, control);
  }

  get valueFC() {
    return this.get(InfosFCN.value) as FormControl;
  }

  set valueFC(control: FormControl) {
    this.setControl(InfosFCN.value, control);
  }

  get typeFC() {
    return this.get(InfosFCN.type) as FormControl;
  }

  set typeFC(control: FormControl) {
    this.setControl(InfosFCN.type, control);
  }

  get modifiableFC() {
    return this.get(InfosFCN.modifiable) as FormControl;
  }

  set modifiableFC(control: FormControl) {
    this.setControl(InfosFCN.modifiable, control);
  }

  get btnTxtFC() {
    if (this.type != IArgType.configfile)
      return "set"
    else
      return "download"
  }
}

