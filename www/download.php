<?
$what = urldecode($_GET['what']);
require_once('func.php');


// invalid dir...
if( strstr($what, "..") !== false )
{
  header("HTTP/1.1 404 Not Found");
  exit;
}
$fpath = 'download/'.$what;
if( !file_exists($fpath) )
{
  header("HTTP/1.1 404 Not Found");
  exit;
}
counter_inc($what);
sendfile($fpath);
?>
