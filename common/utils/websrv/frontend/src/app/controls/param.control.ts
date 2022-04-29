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
    const doc: IParam = {
      value: ((this.param.col?.type !== IArgType.boolean) ? this.value as string : String(this.value)),
      col: this.param.col
    };

    return doc;
  }
}

