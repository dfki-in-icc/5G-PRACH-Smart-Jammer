import { ComponentFactoryResolver } from '@angular/core';
import { FormControl } from '@angular/forms';
import { IArgType, IParam } from '../api/commands.api';


export class ParamFC extends FormControl {

  constructor(
    public param: IParam
  ) {
    super((param.col!.type !== IArgType.boolean)
      ? param.value
      : ((param.value === 'true') ? true : false)
    );
  }

  api() {

    let value: string

    switch (this.param.col?.type) {

      case IArgType.string:
      case IArgType.boolean:
        value = this.value as string;
        break;

      case IArgType.list:
      case IArgType.configfile:
      case IArgType.range:
      case IArgType.number:

      default:
        value = String(this.value)
    }


    const doc: IParam = {
      value: value,
      col: this.param.col
    };

    return doc;
  }
}

