import copy
import prettytable as pt
# 终结符号用ε表示，栈顶用#来进行表示
'''
**************单词符号及其分类编码****************
单词符号	      种别码
=               1
(				2
)				3
+				4
-				5
*				6
/				7
i				8
#				9
**********************************************
'''

# 用于存储follow表
follow_table = {}


# 返回FIRST所有的状态
def get_state(GS):
    first = []
    for i in GS:
        if i[0] not in first:
            first.append(i[0])
    return first


# 返回运算后的FIRST集
def get_FIRST(GS):
    # 获取FIRST中的状态集合
    state = get_state(GS)
    # 初始化FIRST表
    first_table = [[i] for i in state]
    for i in range(len(first_table)):
        result = recur_get_this_state(first_table[i][0], GS)
        first_table[i].extend(result)
    return first_table


# 递归循环得到FIRST集
def recur_get_this_state(target, GS):
    result = []
    for i in range(len(GS)):
        # 当前推导符号和要进行递归的符号相同
        if target == GS[i][0]:
            # 判断是否为终结符
            if GS[i][1][0].islower() or GS[i][1][0].isalpha() == False:
                add_into_result(result, GS[i][1][0])

            else:
                # 如果为非终结符号
                temp = recur_get_this_state(GS[i][1][0], GS)
                # 用于标记
                index = 0
                # 若有非终结符存在
                while ("ε" in temp):
                    for q in range(0, len(temp)):
                        if temp[q] == "ε":
                            del temp[q]
                            break
                    add_into_result(result, temp)
                    index += 1
                    if index == len(str(GS[i][1])):
                        add_into_result(result, "ε")
                        break
                    else:
                        temp = recur_get_this_state(GS[i][1][index], GS)
                add_into_result(result, temp)

    return result


# 向list中添加不重复的元素
def add_into_result(old, add):
    for i in add:
        if i not in old:
            old.extend(i)


# 得到follow集
def get_FOLLOW(GS, first):
    global follow_table
    # 获取FIRST中的状态集合
    state = get_state(GS)
    # 初始化FIRST表
    for i in range(len(state)):
        follow_table[str(state[i])] = []
    follow_table[str(state[0])].extend("#")
    for i in range(len(follow_table)):
        result = recur_get_follow(str(state[i]), GS, first)
        add_into_result(follow_table[str(state[i])], result)
    return follow_table


# 递归得到follow集
def recur_get_follow(target, GS, first):
    result = []
    for i in range(len(GS)):
        for a in range(1, len(GS[i])):
            # 查看是否包含当前字符
            if str(GS[i][a]).find(target) != -1:
                # 获得当前字符所在的位置，指向当前字符
                index = str(GS[i][a]).find(target)
                # 当前字符后面跟有其他字符
                if index != len(GS[i][a]) - 1:
                    # 如果为终结符号的话
                    index += 1
                    if GS[i][a][index].islower() or GS[i][a][index].isalpha() == False:
                        add_into_result(result, GS[i][a][index])
                    else:
                        # 如果为非终结符号，则进行判断是FIRST还是FOLLOW集加入
                        while (index != len(GS[i][a])):
                            # 如果在FIRST集中有空符号出现，则该FIRST集加入到FOLLOW集当中，并且读取下一个符号
                            if "ε" in first[str(GS[i][a][index])]:
                                new = first[str(GS[i][a][index])]
                                for x in new:
                                    if x != "ε":
                                        add_into_result(result, x)
                            else:
                                # 当前FIRST集中没有空符号，则该FOLLOW集添加完毕，退出循环
                                add_into_result(result, first[str(GS[i][a][index])])
                                break
                            index += 1
                        # 如果index等于当前符号跳转位的长度，说明该符号跳转位到最后一位为止都在进行判别，则此时把
                        # 跳转符号的FOLLOW集加入到目标FOLLOW集当中
                        if index == len(GS[i][a]):
                            if follow_table[str(GS[i][0])] == []:
                                rea = recur_get_follow(str(GS[i][0]), GS, first)
                                add_into_result(result, rea)
                            else:
                                add_into_result(result, follow_table[str(GS[i][0])])
                else:
                    # 当前字符是最后一个字符
                    # 根据该推导字符是否已经被推导过了来决定是否进行循环
                    if follow_table[str(GS[i][0])] == []:
                        rea = recur_get_follow(str(GS[i][0]), GS, first)
                        add_into_result(result, rea)
                    else:
                        add_into_result(result, follow_table[str(GS[i][0])])
                        add_into_result(follow_table[target], result)
    return result


# 计算得到select集
def get_SELECT(GS, first, follow):
    # 初始化select表
    select = [[[a, b], []] for a, b in GS]
    # 用来进行标记FIRST合并的符号
    flag = False
    # 存储非终结符号里面的结果
    new_add = []
    # 依次查看跳转的每一条语句
    for i in range(len(select)):
        # 查看跳转的每个字符
        for a in select[i][0][1]:
            # 如果为非终结符号，则加入到select集中，并不再进行接下来的判断
            if str(a).islower() or str(a).isalpha() == False:
                add_into_result(new_add, a)
                break
            else:
                # 如果为非终结符号
                # 查看非终结符号所含有的FIRST集中的集合是否含有空符号
                # 若有则读取跳转符号的follow集

                # 挨个去除FIRST集，并按规定去除里面含有的空集
                if new_add == []:
                    new_add = copy.deepcopy(first[str(a)])
                    if "ε" not in new_add:
                        break
                else:
                    # 假如是第二次进行FIRST集合并则需要去除其中一个的ε符号
                    if flag == False:
                        if "ε" in new_add:
                            for x in range(len(new_add)):
                                if new_add[x] == "ε":
                                    del new_add[x]
                                    break
                            add_into_result(new_add, first[str(a)])
                        elif "ε" in first[str(a)]:
                            for i in first[str(a)]:
                                if i != "ε":
                                    add_into_result(new_add, i)
                        flag = True
                    elif "ε" in first[str(a)]:
                        for i in first[str(a)]:
                            if i != "ε":
                                add_into_result(new_add, i)
        # 如果FIRST集中有空符号，则去除空符号，并将FOLLOW集加入
        if "ε" in new_add:
            for f in range(len(new_add)):
                if new_add[f] == "ε":
                    del new_add[f]
                    break
            add_into_result(new_add, follow[str(select[i][0][0])])

        # 将结果加入到select集中
        select[i][1].extend(new_add)
        # 重置变量初始值
        new_add = []
        flag = False

    return select


# 判断两个select是否含有交集
# 有交集返回True，没有交集返回False
def judge_select(select):
    for a1 in select:
        for a2 in select:
            # 判断两个select集是有否相同左部产生式的
            if a1[0][0] == a2[0][0] and a1[0][1] != a2[0][1]:
                intersection = list(set(a1[1]).intersection(set(a2[1])))
                if intersection != []:
                    return True
    return False


# 返回select集合中的终结符号集合
def get_symbol1(select):
    result1 = []
    result2 = []
    for i in select:
        for x in i[0][0]:
            if x not in result1:
                result1.extend(x)
        for x in i[1]:
            if x not in result2:
                result2.extend(x)
    return result1, result2


# 得到预测分析表
def predicting_analysis(select):
    # 得到预测分析表的终结符号和非终结符号的集合
    state, symbol = get_symbol1(select)
    # 初始化预测分析表
    table = [[0 for col in range(len(symbol))] for row in range(len(state))]
    # 构造预测分析表
    for i in range(len(state)):
        for x in range(len(select)):
            # 如果推导符号相同
            if select[x][0][0] == state[i]:
                for e in range(len(symbol)):
                    # 如果该推导式能推导该符号
                    if symbol[e] in select[x][1]:
                        table[i][e] = [select[x][0][1]]

    return state, symbol, table


# 预测分析
# state:预测分析表的行名
# symbol:预测分析表的列名
# table:预测分析表
# str:输入字符串
# start:起始字符
def analy_str(state, symbol, table, istr, start):
    # 分析栈
    analy_shed = []
    # 输入串栈
    str_shed = []

    # 初始化栈
    analy_shed.append("#")
    analy_shed.append(start)
    str_shed.append("#")
    str_shed.extend(list(istr[::-1]))
    while(str_shed[-1] != "#" or analy_shed[-1] != "#"):
        # 查看栈顶是否相同，相同则匹配消去
        if str_shed[-1] == analy_shed[-1]:
            str_shed.pop()
            analy_shed.pop()
        else:
            # 当栈顶不同的时候，看能否进行产生式推导
            y,x = arithmetic(state,symbol,analy_shed[-1],str_shed[-1])
            if table[y][x] == 0:
                return "this input is illegal"
            elif table[y][x][0] == "ε":
                analy_shed.pop()
            else:
                analy_shed.pop()
                # 推导的表达式入栈
                analy_shed.extend(list((table[y][x][0])[::-1]))
    return "this input can be derivated by LL(1)"


# 根据两边的栈顶进行产生式的寻找
# state:预测分析表的行名
# symbol:预测分析表的列名
# analy:分析栈的栈顶元素
# str:剩余输入串的栈顶元素
def arithmetic(state, symbol, analy, str):
    for a in range(len(state)):
        for b in range(len(symbol)):
            # 如果根据栈顶可以推导出新的产生式，则进行推导
            if state[a] == analy and symbol[b] == str:
                # 返回产生式在预测分析表里面的位置
                return a, b
    return -1,-1

def error():
    print("Syntax error")
    print("This input is not an expression of the language!")

def error1():
    print("illegal input error！")
    print("This input is not an expression of the language!")


def judge(symbol):
    chart = {
        '1': "=",
        '2': "(",
        '3': ")",
        '4': "+",
        '5': "-",
        '6': "*",
        '7': "/",
        '8': "i",
        '9': "#",
    }
    return chart.get(symbol)

# 返回每个二元式第二项合并成的字符串
def read_file():
    #读取二元式txt文件
    with open('input1.txt', 'r') as f1:
        list1 = f1.readlines()

    list1_symbol = []
    list1_new = []
    for i in range(0, len(list1)):
        list1[i] = list1[i].rstrip('\n')    #去除每行结尾的回车符
        list1_symbol.append(list1[i].__getitem__(1))  # 提取每个二元式的第二项
        list1_new.append(list1[i].__getitem__(3))   #提取每个二元式的第二项

    #判断文件读取内容是否合法，包括检查非法字符和不匹配现象
    for symbol,value in zip(list1_symbol,list1_new):
        if judge(symbol) != value:
            error1()    #出现非法字符则报错

    # 返回每个二元式第二项合并成的字符串
    return "".join(list1_new)


if __name__ == '__main__':
    # 初始化文法
    GS = [["S","V=E"],
          ["E", "TP"],      #P表示E1
          ["P", "ATP"],
          ["P", "ε"],
          ["T", "FQ"],      #Q表示T1
          ["Q", "MFQ"],
          ["Q", "ε"],
          ["F", "(E)"],
          ["F", "i"],
          ["A","+"],
          ["A","-"],
          ["M","*"],
          ["M","/"],
          ["V","i"]]

    # 输入串，即词法分析的输出二元式序列
    input_str = read_file()

    # 得到FIRST集
    first = {}
    output = get_FIRST(GS)
    for i in range(len(output)):
        first[str(output[i][0][0])] = output[i][1:]
    # 得到FOLLOW集
    follow = get_FOLLOW(GS, first)
    print("GS's first:")
    print(first)
    print("GS's follow:")
    print(follow)
    # 得到select集
    select = get_SELECT(GS, first, follow)
    print("GS's select:")
    for i in select:
        print(i)

    # 根据select的交集进行判断是否为LL(1)文法
    if judge_select(select):
        print("this is not LL(1)")     #因为相同左部产生式的select集的交集不为空
    else:
        print("this is LL(1)")

        # 得到预测分析表
        state, symbol, table = predicting_analysis(select)

        # 预测分析
        result = analy_str(state, symbol, table, input_str, GS[0][0])
        print(result)

    #输出LL(1)分析表
    tb = pt.PrettyTable()
    tb.field_names = ["i", "+", "-", "*", "/", "(", ")", "#"]

    table_new = []
    for i in table:
        tb.add_row(i)
    print("LL(1)  table :")
    print(tb)
