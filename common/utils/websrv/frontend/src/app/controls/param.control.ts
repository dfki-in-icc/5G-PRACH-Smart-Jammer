import { FormControl, FormGroup } from "@angular/forms";
import { IArgType, IColumn, IParam } from "../api/commands.api";


enum ParamFCN {
  value = 'value',
}


export class ParamCtrl extends FormGroup {

  col: IColumn
  constructor(public param: IParam) {
    super({})

    this.col = param.col

    let control: FormControl
    switch (param.col.type) {
      case IArgType.boolean:
        control = new FormControl((param.value === 'true') ? true : false);
        break;

      case IArgType.loglvl:
        control = new FormControl(param.value);
        break;

      default:
        control = new FormControl(param.value)
    }

    if (!param.col.modifiable) control.disable()

    this.addControl(ParamFCN.value, control)
  }

  get valueFC() {
    return this.get(ParamFCN.value) as FormControl
  }

  set valueFC(fc: FormControl) {
    this.setControl(ParamFCN.value, fc);
  }

  api() {
    let value: string

    switch (this.col.type) {
      case IArgType.boolean:
        value = String(this.valueFC.value);
        break;

      default:
        value = this.valueFC.value
    }

    const doc: IParam = {
      value: value,
      col: this.col
    }

    return doc

  }

}

