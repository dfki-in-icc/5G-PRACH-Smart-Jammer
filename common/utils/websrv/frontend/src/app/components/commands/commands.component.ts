import { Component } from '@angular/core';
import { AbstractControl, FormArray } from '@angular/forms';
import { BehaviorSubject, forkJoin, timer } from 'rxjs';
import { Observable } from 'rxjs/internal/Observable';
import { of } from 'rxjs/internal/observable/of';
import { filter } from 'rxjs/internal/operators/filter';
import { map } from 'rxjs/internal/operators/map';
import { tap } from 'rxjs/internal/operators/tap';
import { CommandsApi, IArgType, IColumn, ICommand, IInfo, IParam, IRow } from 'src/app/api/commands.api';
import { CmdCtrl } from 'src/app/controls/cmd.control';
import { InfoCtrl } from 'src/app/controls/info.control';
import { ModuleCtrl } from 'src/app/controls/module.control';
import { RowCtrl } from 'src/app/controls/row.control';
import { VarCtrl } from 'src/app/controls/var.control';
import { DialogService } from 'src/app/services/dialog.service';
import { DownloadService } from 'src/app/services/download.service';
import { LoadingService } from 'src/app/services/loading.service';



@Component({
  selector: 'app-commands',
  templateUrl: './commands.component.html',
  styleUrls: ['./commands.component.css'],
})
export class CommandsComponent {

  IArgType = IArgType;

  // softmodem
  infos$: Observable<VarCtrl[]>
  modules$: Observable<ModuleCtrl[]>

  // module
  selectedModule?: ModuleCtrl
  vars$?: Observable<VarCtrl[]>
  cmds$?: Observable<CmdCtrl[]>

  // command
  selectedCmd?: ICommand
  displayedColumns: string[] = []
  rows$: BehaviorSubject<RowCtrl[]> = new BehaviorSubject<RowCtrl[]>([])
  columns: IColumn[] = []

  constructor(
    public commandsApi: CommandsApi,
    public loadingService: LoadingService,
    public dialogService: DialogService,
    public downloadService: DownloadService,

  ) {
    this.infos$ = this.commandsApi.readInfos$().pipe(
      map((infos) => infos.map(info => new InfoCtrl(info)))
    );

    this.modules$ = this.commandsApi.readModules$().pipe(
      map(imodules => imodules.map(imodule => new ModuleCtrl(imodule))),
      filter(controls => controls.length > 0),
      tap(controls => this.onModuleSelect(controls[0]))
    );
  }

  onInfoSubmit(control: InfoCtrl) {
    let info: IInfo = control.api();

    if (info.type === IArgType.configfile) {
      this.downloadService.getFile(info.value)
    } else {
      this.commandsApi.setInfo$(info).subscribe();
    }
  }

  onModuleSelect(module: ModuleCtrl) {

    this.selectedModule = module
    this.selectedCmd = undefined

    this.cmds$ = this.commandsApi.readCommands$(module.name).pipe(
      map(icmds => icmds.map(icmd => new CmdCtrl(icmd))),
      map(cmds => {
        module.cmdsFA = new FormArray(cmds)
        return module.cmdsFA.controls as CmdCtrl[]
      })
    )

    this.vars$ = this.commandsApi.readVariables$(module.name).pipe(
      map(ivars => ivars.map(ivar => new VarCtrl(ivar))),
      map(vars => {
        module.varsFA = new FormArray(vars)
        return module.varsFA.controls as VarCtrl[]
      })
    )
  }

  onVarsubmit(control: VarCtrl) {
    this.commandsApi.setCmdVariable$(control.api(), this.selectedModule!.name)
      .pipe(
        map(resp => this.dialogService.openVarRespDialog(resp))
      ).subscribe();
  }

  onCmdSubmit(control: CmdCtrl) {

    this.selectedCmd = control.api()

    const obsparam = forkJoin([control.confirm
      ? this.dialogService.openConfirmDialog(control.confirm)
      : control.question
        ? this.dialogService.openQuestionDialog(this.selectedModule! + " " + this.selectedModule!.name, control)
        : of(true)]);

    obsparam.subscribe(results => {
      if (!results[0])
        return of(null);
      if (control.isResUpdatable())
        control.ResUpdTimer = timer(2000, 1000);
      return this.execCmd(control);
    });

    // subscribe obsparam results
    return of(null);
  }

  private execCmd(control: CmdCtrl) {
    if (control.isResUpdatable() && control.ResUpdTimer) {
      if (!(control.ResUpdTimerSubscriber)) {
        control.ResUpdTimerSubscriber = control.ResUpdTimer.subscribe(iteration => {
          console.log("Update timer fired" + iteration);
          if (control.updbtnname === "Stop update")
            this.execCmd(control);
        });
      }
    }

    this.commandsApi.runCommand$(control!.api(), this.selectedModule!.name).subscribe(resp => {
      if (resp.display[0]) this.dialogService.updateCmdDialog(control, resp, 'cmd ' + control.nameFC.value + ' response:')
      //          else return of(resp)
      const controls: RowCtrl[] = []
      this.displayedColumns = []

      if (resp.table) {
        this.columns = resp.table.columns
        this.displayedColumns = this.columns.map(col => col.name)
        this.displayedColumns.push('button')

        for (let rawIndex = 0; rawIndex < resp.table.rows.length; rawIndex++) {

          let params: IParam[] = []
          for (let i = 0; i < this.columns.length; i = i + 1) {
            params.push({
              value: resp.table.rows[rawIndex][i],
              col: this.columns[i]
            })
          }

          const irow: IRow = {
            params: params,
            rawIndex: rawIndex,
            cmdName: this.selectedCmd!.name
          }

          controls[rawIndex] = new RowsFA(irow)
        }
      }
      this.rows$.next(controls)
    },
      err => console.error('execCmd error: ' + err),
      () => console.log('execCmd completed: '),
    ) // map resp


    return of(null);
  }

  onParamSubmit(control: RowCtrl) {
    this.commandsApi.setCmdParams$(control.api(), this.selectedModule!.name).subscribe(() => this.execCmd(new CmdCtrl(this.selectedCmd!)));
  }


}
