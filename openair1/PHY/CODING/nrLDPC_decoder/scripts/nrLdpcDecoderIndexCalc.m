% Script to create index tables from base graphs

% Read BGs from xlsx file
% BG1
columnIndex_BG1   = xlsread('nrLDPC_BG1.xlsx',1);
numColsPerRow_BG1 = xlsread('nrLDPC_BG1.xlsx',2);
shiftValues_BG1   = xlsread('nrLDPC_BG1.xlsx',3);
% BG2
columnIndex_BG2   = xlsread('nrLDPC_BG2.xlsx',1);
numColsPerRow_BG2 = xlsread('nrLDPC_BG2.xlsx',2);
shiftValues_BG2   = xlsread('nrLDPC_BG2.xlsx',3);

% Size of BGs (Mb x Nb)
Mb1 = 46;
Nb1 = 68;
Mb2 = 42;
Nb2 = 52;

% Sets if LDPC lifting sizes
liftingSizes = [ 2  4  8  16  32  64 128 256 ; ...
                 3  6 12  24  48  96 192 384 ; ...
                 5 10 20  40  80 160 320   0 ; ...
                 7 14 28  56 112 224   0   0 ; ...
                 9 18 36  72 144 288   0   0 ; ...
                11 22 44  88 176 352   0   0 ; ...
                13 26 52 104 208   0   0   0 ; ...
                15 30 60 120 240   0   0   0 ];

NR_LDPC_ZMAX = 384;

plotBG = 0;

% Parameters for BG selection
BG = 2;
Z = 384;
R = 23;

switch R
    case 15
        r = 1/5;
    case 13
        r = 1/3;
    case 23
        r = 2/3;
    case 89
        r = 8/9;
    otherwise
        r = 1/5;
end

% allZ = sort(liftingSizes(liftingSizes>0));
allZ = 2;
group = 4;
for iZ = 1:length(allZ)
    Z = allZ(iZ);

% Select lifting set index
[iLS, ~] = find(liftingSizes == Z);

%% Create BG matrix
if (BG == 1)
    columnIndex   = columnIndex_BG1;
    numColsPerRow_BG = numColsPerRow_BG1;
    shiftValues   = shiftValues_BG1(:,iLS);
    Mb = Mb1;
    Nb = Nb1;
    K  = 22;
    NR_LDPC_START_COL_PARITY = 26;
elseif (BG == 2)
    columnIndex   = columnIndex_BG2;
    numColsPerRow_BG = numColsPerRow_BG2;
    shiftValues   = shiftValues_BG2(:,iLS);
    Mb = Mb2;
    Nb = Nb2;
    K  = 10;
    NR_LDPC_START_COL_PARITY = 14;
else
    error('Unknown BG');
end
shiftValues = mod(shiftValues, Z);


% Calculate effective BG
% fprintf('<Parameters>\n');
% fprintf('BG = %d\n', BG);
% fprintf('R  = %d\n', R);

Nb = ceil(K/r) + 2;
Mb = Nb - K;
numColsPerRow = numColsPerRow_BG(1:Mb);

Hb = -ones(Mb, Nb);
Hb(1,columnIndex(1:numColsPerRow(1))+1) = shiftValues(1:numColsPerRow(1));
for irow = 2:Mb
    idx = sum(numColsPerRow(1:(irow-1)))+1:sum(numColsPerRow(1:irow));
    idxCols = columnIndex(idx) + 1;
    
    Hb(irow, idxCols) = shiftValues(idx);
end

if plotBG == 1
    NO_SHIFT = max(shiftValues)+1;
    Hb(Hb==-1) = NO_SHIFT;
    imagesc(Hb);
    cmap = jet(max(Hb(:)));
    cmap(end,:) = ones(1,3);
    colormap(cmap);
    colorbar;
    grid on; xticks(0:5:Nb);
    Hb(Hb==NO_SHIFT) = -1;
end

numCNperBN = sum(Hb~=-1,1);
numBNperCN = sum(Hb~=-1,2);

cnGroups = unique(numBNperCN);
bnGroups = unique(numCNperBN);

numCNperBNGroup = sum((numCNperBN' == bnGroups),1);
numEdgesPerBNGroup = bnGroups.*numCNperBNGroup;

picBG = [numCNperBN;Hb];
picBG = [[0;numBNperCN],picBG];

% Shift values per group
numCNinGroup = sum(numColsPerRow==group);

% Start adresses in BN proc buffer
[x,~] = find(numColsPerRow==group);
y = mod(reshape(find(Hb(x,:)'~=-1),group,numCNinGroup)',Nb);
y(y==0) = Nb;

yUnique = unique(y);

bnProcBuffCpyOffset = (y-1);
bnSubGroup = zeros(group,length(yUnique));

startAddr = zeros(numCNinGroup,group);
cshift = zeros(numCNinGroup,group);
bnPos = zeros(numCNinGroup,group);
for n=1:group
    for k=1:numCNinGroup
        
        bnGroup = numCNperBN(y(k,n));        
        bnGroupIdx = find(bnGroups == numCNperBN(y(k,n)));

        startAddrBnGroup = sum(numEdgesPerBNGroup(1:(bnGroupIdx-1)))*NR_LDPC_ZMAX;        
        offsetCnInBnGroup = numCNperBNGroup(bnGroupIdx) * ((sum(Hb(1:x(k),y(k,n))~=-1)-1)) * NR_LDPC_ZMAX;
%         offsetBnInCn = sum(numCNperBN(:,1:y(k,n)-1)==bnGroup)*Z;
        offsetBnInCn = 0;
        bnPos(k,n) = sum(numCNperBN(:,1:y(k,n)-1)==bnGroup);
        
        startAddr(k,n) = startAddrBnGroup + offsetCnInBnGroup + offsetBnInCn;
        cshift(k,n) = Hb(x(k),y(k,n));
    end
    bnSubGroup(n,:) = sum(yUnique'==y(:,n),1);
end

bnSubGroupNum = zeros(1,length(yUnique));
addrOffset = [];
for i = 1:length(yUnique)
    bnIdx = yUnique(i);
    bnSubGroupNum(i) = sum((bnIdx==y),'all');
    idxY = find(bnIdx==y);
    addrOffset = [addrOffset, NR_LDPC_ZMAX*(idxY-1)'];
end
yUnique = yUnique(:).';
% print_bnSubGroupAddr(addrOffset,BG,R,group,bnSubGroupNum);
bnSubGroupIdx_cell = print_bnSubGroupIdx(BG,R,group,bnSubGroupNum,yUnique);
[idxBnProcBuf,startAddrBnProcBuf,startAddrLlrSG] = print_addrBnProcBuf(y-1,bnSubGroupIdx_cell,bnSubGroupNum,NR_LDPC_ZMAX);
% print_bnSubGroupCshift(BG,R,Z,group,bnSubGroupNum,cshift);
% print_cshift(cshift,BG,Z,group,numCNinGroup);
% print_startAddr(startAddr,BG,R,group,numCNinGroup);
% print_bnPos(bnPos,BG,R,group,numCNinGroup);
 
% LUT for llr2CnProcBuf
print_posBnInCnProcBuf(y-1,BG,group,numCNinGroup);

% LUT for llr2llrProcBuf
% LLR processing buffer org
% bnGroups  |1 |4 |5 |6 |7 |8 |9 |10|11|12|13|28|30|
% numofBnGr |42|1 |1 |2 |4 |3 |1 |4 |3 |4 | 1| 1| 1|
% startAddr       ^                             ^
%              42*Z_MAX                      67*Z_MAX
startAddrLlrProc = tril(ones(length(numCNperBNGroup)))*[0 numCNperBNGroup(1:end-1)]'*NR_LDPC_ZMAX;

colParity = sum(numCNperBN>1);
numCNperBNnoParity = numCNperBN(1:colParity);

startAddr = zeros(length(numCNperBNnoParity),1);
bnPos = zeros(length(numCNperBNnoParity),1);
numBn = ones(length(numCNperBNnoParity),1);
bnGroup = numCNperBNnoParity(1);
bnGroupIdx = find(bnGroups == bnGroup);
% 
% startAddr(1) = startAddrLlrProc(bnGroupIdx);
% idx = 2;
for k=1:length(numCNperBNnoParity)    
    bnGroup = numCNperBNnoParity(k);
    bnGroupIdx = find(bnGroups == bnGroup);
    startAddr(k) = startAddrLlrProc(bnGroupIdx);
    bnPos(k) = sum(numCNperBNnoParity(:,1:k-1)==bnGroup);
    
%     bnPosCur = sum(numCNperBNnoParity(:,1:k-1)==bnGroup);
%     addr = startAddrLlrProc(bnGroupIdx);
%         
%     if (bnGroup == numCNperBNnoParity(k-1))        
%         numBn(idx-1) = numBn(idx-1) + 1;
%     else
%         startAddr(idx) = addr;
%         bnPos(idx) = bnPosCur;
%         idx = idx + 1;
%     end
end
% bnPos = bnPos(startAddr > 0);
% startAddr = startAddr(startAddr > 0);
% 
% numBn = numBn(1:length(startAddr));

% print_llr2llrProcBuf(startAddr,bnPos,numBn,BG,R);

end

function [idxBnProcBuf,startAddrBnProcBuf,startAddrLlrSG] = print_addrBnProcBuf(y,bnSubGroupIdx_cell,bnSubGroupNum,NR_LDPC_ZMAX)

uniqueSG = unique(bnSubGroupNum);
uniqueSGSum = sum((uniqueSG'==bnSubGroupNum),2);

for i=1:length(uniqueSG)
    SG = uniqueSG(i);
    bnInSG = bnSubGroupIdx_cell{i};
    if i==1
        startAddri = 0;
    else
        startAddri = startAddri+((uniqueSGSum(i-1)*uniqueSG(i-1)*NR_LDPC_ZMAX)+(uniqueSGSum(i-1)*NR_LDPC_ZMAX)); % +1 for input LLRs
    end
    if SG == 1      
        a = 0;
        for j=1:length(bnInSG)
            idx = find(bnInSG(j)==y);
            idxBnProcBuf(idx) = a;
            startAddrBnProcBuf(idx) = startAddri;
            a = a+1;
        end
    else
        for j=1:length(bnInSG)
            idx = find(bnInSG(j)==y);
            for k = 1:length(idx)
                idxBnProcBuf(idx(k)) = j-1;
                startAddrBnProcBuf(idx(k)) = startAddri+(k-1)*uniqueSGSum(i)*NR_LDPC_ZMAX;
            end
        end
    end
    startAddrLlrSG(i) = startAddri+((uniqueSGSum(i)*uniqueSG(i)*NR_LDPC_ZMAX)+(uniqueSGSum(i)*NR_LDPC_ZMAX));
end

end

function print_bnSubGroupAddr(addrOffset,BG,R,group,bnSubGroupNum)

prefix = 'static const uint16_t';

uniqueSG = unique(bnSubGroupNum);
uniqueSGSum = sum((uniqueSG'==bnSubGroupNum),2);


for i=1:length(uniqueSG)
    SG = uniqueSG(i);
    idxBnSubGroupNum = find(SG==bnSubGroupNum);
    arrayName = strcat('addrOffset', '_BG', num2str(BG), '_CNG',num2str(group),...
        '_SG',num2str(SG),'_R',num2str(R),'[',num2str(uniqueSGSum(i)),'][',num2str(SG),'] =');
    fprintf('%s %s ',prefix,arrayName);
    fprintf('{{');
    for j=idxBnSubGroupNum(1:end-1)
        s = sum(bnSubGroupNum(1:j-1),2);
        y = addrOffset(s+1:s+SG);
        if ~isempty(y(1:end-1))
            fprintf('%d, ',y(1:end-1));
        end
        fprintf('%d},{',y(end));
    end
    j = idxBnSubGroupNum(end);
    s = sum(bnSubGroupNum(1:j-1),2);
    y = addrOffset(s+1:s+SG);
    if ~isempty(y(1:end-1))
        fprintf('%d, ',y(1:end-1));
    end
    fprintf('%d}',y(end));
    fprintf('};\n');
end
    
end

function bnSubGroupIdx_cell = print_bnSubGroupIdx(BG,R,group,bnSubGroupNum,yUnique)

prefix = 'static const uint16_t';
bnSubGroupIdx_cell={};
uniqueSG = unique(bnSubGroupNum);
uniqueSGSum = sum((uniqueSG'==bnSubGroupNum),2);


for i=1:length(uniqueSG)
    SG = uniqueSG(i);
    bnSubGroupIdx_cell{i} = [];
    idxBnSubGroupNum = SG==bnSubGroupNum;
    arrayName = strcat('bnIdx', '_BG', num2str(BG), '_CNG',num2str(group),...
        '_SG',num2str(SG),'_R',num2str(R),'[',num2str(uniqueSGSum(i)),'] =');
    fprintf('%s %s ',prefix,arrayName);
    fprintf('{');
    y = (yUnique(idxBnSubGroupNum))-1;
    if ~isempty(y(1:end-1))
        fprintf('%d, ',y(1:end-1));
        bnSubGroupIdx_cell{i} = [bnSubGroupIdx_cell{i}, y(1:end-1)];
    end
    fprintf('%d',y(end));
    bnSubGroupIdx_cell{i} = [bnSubGroupIdx_cell{i}, y(end)];
    fprintf('};\n');
end
    
end

function print_bnSubGroupCshift(BG,R,Z,group,bnSubGroupNum,cshift)

prefix = 'static const uint16_t';

uniqueSG = unique(bnSubGroupNum);
uniqueSGSum = sum((uniqueSG'==bnSubGroupNum),2);


for i=1:length(uniqueSG)
    SG = uniqueSG(i);
    idxBnSubGroupNum = SG==bnSubGroupNum;
    arrayName = strcat('bnIdxCshift', '_BG', num2str(BG),'_Z',num2str(Z), '_CNG',num2str(group),...
        '_SG',num2str(SG),'_R',num2str(R),'[',num2str(uniqueSGSum(i)),'] =');
    fprintf('%s %s ',prefix,arrayName);
    fprintf('{');
    y = (cshift(idxBnSubGroupNum));
    if ~isempty(y(1:end-1))
        fprintf('%d, ',y(1:end-1));
    end
    fprintf('%d',y(end));
    fprintf('};\n');
end
    
end

function print_posBnInCnProcBuf(y,BG,group,numCNinGroup)

prefix = 'static const uint8_t';

arrayName = strcat('posBnInCnProcBuf', '_BG', num2str(BG), '_CNG',num2str(group),...
    '[',num2str(group),'][',num2str(numCNinGroup),'] =');

fprintf('%s %s ',prefix,arrayName);
fprintf('{{');
for n=1:group-1
    if ~isempty(y(1:end-1,n))
        fprintf('%d, ',y(1:end-1,n));
    end
    fprintf('%d},{',y(end,n));
end
if ~isempty(y(1:end-1,n))
    fprintf('%d, ',y(1:end-1,group));
end
fprintf('%d}};\n',y(end,group));

end

function print_llr2llrProcBuf(startAddr,bnPos,numBn,BG,R)

N = length(startAddr);
prefix = 'static const uint16_t';

arrayName = strcat('llr2llrProcBufAddr', '_BG', num2str(BG),'_R', num2str(R),...
    '[',num2str(N),'] =');

fprintf('%s %s ',prefix,arrayName);
fprintf('{');
fprintf('%d,',startAddr(1:end-1));
fprintf('%d};\n',startAddr(end));

prefix = 'static const uint8_t';

arrayName = strcat('llr2llrProcBufBnPos', '_BG', num2str(BG),'_R', num2str(R),...
    '[',num2str(N),'] =');

fprintf('%s %s ',prefix,arrayName);
fprintf('{');
fprintf('%d,',bnPos(1:end-1));
fprintf('%d};\n',bnPos(end));

prefix = 'static const uint8_t';

arrayName = strcat('llr2llrProcBufNumBn', '_BG', num2str(BG),'_R', num2str(R),...
    '[',num2str(N),'] =');

fprintf('%s %s ',prefix,arrayName);
fprintf('{');
fprintf('%d,',numBn(1:end-1));
fprintf('%d};\n',numBn(end));
end

function print_bnPos(bnPos,BG,R,group,numCNinGroup)

prefix = 'static const uint8_t';

arrayName = strcat('bnPosBnProcBuf', '_BG', num2str(BG),'_R', num2str(R), '_CNG',num2str(group),...
    '[',num2str(group),'][',num2str(numCNinGroup),'] =');

fprintf('%s %s ',prefix,arrayName);
fprintf('{{');
for n=1:group-1
    if ~isempty(bnPos(1:end-1,n))
        fprintf('%d, ',bnPos(1:end-1,n));
    end
    fprintf('%d},{',bnPos(end,n));
end
if ~isempty(bnPos(1:end-1,n))
    fprintf('%d, ',bnPos(1:end-1,group));
end
fprintf('%d}};\n',bnPos(end,group));
end

function print_startAddr(addr,BG,R,group,numCNinGroup)

prefix = 'static const uint32_t';

arrayName = strcat('startAddrBnProcBuf', '_BG', num2str(BG), '_R', num2str(R),'_CNG',num2str(group),...
    '[',num2str(group),'][',num2str(numCNinGroup),'] =');

fprintf('%s %s ',prefix,arrayName);
fprintf('{{');
for n=1:group-1
    if ~isempty(addr(1:end-1,n))
        fprintf('%d, ',addr(1:end-1,n));
    end
    fprintf('%d},{',addr(end,n));
end
if ~isempty(addr(1:end-1,n))
    fprintf('%d, ',addr(1:end-1,group));
end
fprintf('%d}};\n',addr(end,group));
end


function print_cshift(cshift,BG,Z,group,numCNinGroup)
    prefix = 'static const uint16_t';
arrayName = strcat('circShift', '_BG', num2str(BG), '_Z', num2str(Z), '_CNG',num2str(group),...
    '[',num2str(group),'][',num2str(numCNinGroup),'] =');

fprintf('%s %s ',prefix,arrayName);
fprintf('{{');
for n=1:group-1
    if ~isempty(cshift(1:end-1,n))
        fprintf('%d, ',cshift(1:end-1,n));
    end
    fprintf('%d},{',cshift(end,n));
end
if ~isempty(cshift(1:end-1,n))
    fprintf('%d, ',cshift(1:end-1,group));
end
fprintf('%d}};\n',cshift(end,group));
end

