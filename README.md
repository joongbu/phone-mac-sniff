<html>
<meta charset="utf-8">
<head>
<style>
body, html {
  height: 100%;
  padding: 0;
  margin: 0;
}
table, th, td {
  border: 1px solid black;
}
</style>
</head>
<body>
<div style="text-align:center">

<h1><strong>CCIT 출석기록부</strong></h1>

<br>
<form action="pp.php?"method = "GET">
<select name = "search" selected>
<option value = entire>선택</option>
<option value = name>이름</option>
<option value = mac>맥주소</option>
<option value = year>년도</option>      
<option value = month>월</option> 
<option value = day>일</option>
<option value = hour>시간</option>
<option value = minute>분</option>
</select>
<tr>
<td align=center style="width:50px;height:50px;"><input type="text" name="searching_box" style="width:200px;"></td>
</tr>
<input type = submit value = search>
</form>


<?php
mysql_connect('localhost','root','123');
mysql_query("set name utf8");
mysql_select_db('student_data');
$searching = $_GET['searching_box'];
$search = $_GET['search'];
$page = ($_GET['page'])?$_GET['page']:1;
$list = 20;
$block = 3;

if ($_GET['search'] == 'name'){
        $data = mysql_query("select * from timelog where name like '$searching'");
        
        } elseif ($_GET['search'] == 'mac') {
        $data = mysql_query("select * from timelog where mac_addr like '$searching'");
        
        } elseif ($_GET['search'] == 'year') {
        $data = mysql_query("select * from timelog where year like $searching");
        
        } elseif ($_GET['search'] == 'day') {
        $data = mysql_query("select * from timelog where day like $searching");
        
        }
        elseif ($_GET['search'] == 'month') {
        $data = mysql_query("select * from timelog where mon like $searching");
        
        }
        elseif ($_GET['search'] == 'minute') {
        $data = mysql_query("select * from timelog where minute like $searching");
        
        }
        elseif ($_GET['search'] == 'hour') {
        $data = mysql_query("select * from timelog where hour like $searching");
        
        }
        
        else{
        $data = mysql_query("select * from timelog");        
                }

$num = mysql_num_rows($data);
$pageNum = ceil($num/$list);
$blockNum = ceil($pageNum/$block);
$nowBlock = ceil($page/$block);

$s_page = ($nowBlock * $block) -2;

if($_GET[page] && $_GET[page] > 0)
$page= $_GET['page'];
else
$page=1;

$e_page = $nowBlock*$block;
if ($pageNum <= $e_page) {
    $e_page = $pageNum;
}

?>
<br>
<div style="position: center;align:center; bottom: 200px;">
<?php
    if($s_page > 1)
{
?>
    <a href="<?=$PHP_SELP?>?page=<?=$s_page-1?>&search=<?=$search?>&searching_box=<?="$searching"?>">이전</a>
<?php
}
?>

<?php
for ($p=$s_page; $p<=$e_page; $p++) {
?>
<a href="<?=$PHP_SELP?>?page=<?=$p?>&search=<?=$search?>&searching_box=<?="$searching"?>"><?=$p?></a>
<?php
if($p == $e_page)
    {
?>
<a href="<?=$PHP_SELP?>?page=<?=$e_page+1?>&search=<?=$search?>&searching_box=<?="$searching"?>">다음</a>
<?php
}
}
?>
</div>
</br>
<table align="center">
<?php
$s_point = ($page-1) * $list;


    echo "<tr>";
    echo "<th>년도</th>";
    echo "<th>월</th>";
    echo "<th>일</th>";
    echo "<th>시</th>";
    echo "<th>분</th>";
    echo "<th>이름</th>";
    echo "<th>맥주소</th>";
    echo "<th>출석</th>";
    echo "</tr>";

if ($_GET['search'] == 'name'){
        $query = "select * from timelog where name like '$searching' LIMIT $s_point,$list";
        
        } elseif ($_GET['search'] == 'mac') {
        $query = "select * from timelog where mac_addr like '$searching' LIMIT $s_point,$list";
        
        } elseif ($_GET['search'] == 'year') {
        $query = "select * from timelog where year like $searching LIMIT $s_point,$list";
        
        } elseif ($_GET['search'] == 'day') {
        $query = "select * from timelog where day like $searching LIMIT $s_point,$list";
        
        }
        elseif ($_GET['search'] == 'month') {
        $query = "select * from timelog where mon like $searching  LIMIT $s_point,$list";
        
        }
        elseif ($_GET['search'] == 'minute') {
        $query = "select * from timelog where minute like $searching  LIMIT $s_point,$list";
        
        }
        elseif ($_GET['search'] == 'hour') {
        $query = "select * from timelog where hour like $searching LIMIT $s_point,$list";
        
        }
        
        else{
        $query = "select * from timelog LIMIT $s_point,$list";        
                }

        
$result = mysql_query($query);
echo "<br>";
    
for ($i=1; $i<=$num; $i++) {
    $fetch = mysql_fetch_array($result);
echo "<tr>";
    echo "<td>$fetch[0]</td>";
    echo "<td>$fetch[1]</td>";
    echo "<td>$fetch[2]</td>";
    echo "<td>$fetch[3]</td>";
    echo "<td>$fetch[4]</td>";
    echo "<td>$fetch[5]</td>";
    echo "<td>$fetch[6]</td>";
    echo "<td>$fetch[7]</td>";
    echo "</tr>";
?>

    <div>
        <?= $fetch['list_no'] ?>
    </div>

<?php
    if ($fetch == false) {
        exit;
    }
}

?>
</table>
</br>

</div>
</body>

</html> 
