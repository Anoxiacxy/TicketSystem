/** 
 * 参数列表
 * -c -u -p -n -m -g
 * 说明
 * 当前用户<cur_username>为-c，创建一个<username>为-u，<password>为-p，<name>为-n，<mailAddr>为-m，<privilege>为-g的用户。
 * 权限要求：只有当新用户的权限不高于当前用户的权限时。
 * 特别地，创建第一个用户时，新用户权限为10，不受上述权限规则约束。
 * 返回值
 * 注册成功：0
 * 注册失败：-1
 * */
int add_user(int argc, char const *argv[]) {

}
/**
 * 参数列表
 * -u -p
 * 说明
 * 以-u为<username>，-p为<password>登录。若登录成功，当前用户列表增加此用户。第一次进入系统时，当前用户为空。
 * 返回值
 * 登录成功：0
 * 登录失败：-1
 * */
int login(int argc, char const *argv[]) {

}
/**
 * 参数列表
 * -u
 * 说明
 * <username>为-u的用户退出登录。若退出成功，将此用户移出当前用户列表。
 * 返回值
 * 登出成功：0
 * 登出失败：-1
 * */
int logout(int argc, char const *argv[]) {

}

/**
 * 参数列表
 * -c -u
 * 说明
 * <username>为-c的用户查询<username>为-u的用户信息。
 * 权限要求：-c已登录，且（-c的权限大于-u的权限，或是-c和-u相同）。
 * 返回值
 * 查询成功：一行字符串，依次为被查询用户的<username>，<name>，<mailAddr>和<privilege>，用一个空格隔开。
 * 询失败：-1
*/
int query_profile(int argc, char const *argv[]) {
    
}

int main(int argc, char const *argv[])
{
    /* code */

    return 0;
}
