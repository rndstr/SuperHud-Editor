<?
define('DBFILE', '/home/www/web218/files/superhudeditor.dat');

/// @arg $fname the file we are downloading
function counter_inc($fname)
{
  clearstatcache();
  ignore_user_abort(true);    ## prevent refresh from aborting file operations and hosing file
  if (file_exists(DBFILE)) {
    $fh = @fopen(DBFILE, 'r+');
    if( $fh !== false )
     while(1) {
       if (flock($fh, LOCK_EX)) {
           #$buffer = chop(fgets($fh, 2));
           $buffer = chop(fread($fh, filesize(DBFILE)));
           $db = unserialize($buffer);
           if( isset($db[$fname]) )
             ++$db[$fname];
           else
             $db[$fname] = 1;
           $buffer = serialize($db);
           rewind($fh);
           fwrite($fh, $buffer);
           fflush($fh);
           ftruncate($fh, ftell($fh));
           flock($fh, LOCK_UN);
           break;
       }
     }
  }
  else {
     $fh = fopen(DBFILE, 'w+');
     $db[$fname] = 1;
     $buffer = serialize($db);
     fwrite($fh, $buffer);
  }
  fclose($fh);
  ignore_user_abort(false);
}

function sendfile($fpath)
{
  $fsize = filesize($fpath);
  $fname = basename($fpath);
  $bufsize = 20000;

  if(isset($_SERVER['HTTP_RANGE']))  //Partial download
  {
     if(preg_match("/^bytes=(\\d+)-(\\d*)$/", $_SERVER['HTTP_RANGE'], $matches)) { //parsing Range header
         $from = $matches[1];
         $to = $matches[2];
         if(empty($to))
         {
             $to = $fsize - 1;  // -1  because end byte is included
                                 //(From HTTP protocol:
  // 'The last-byte-pos value gives the byte-offset of the last byte in the range; that is, the byte positions specified are inclusive')
         }
         $content_size = $to - $from + 1;

         header("HTTP/1.1 206 Partial Content");
         header("Content-Range: $from-$to/$fsize");
         header("Content-Length: $content_size");
         header("Content-Type: application/force-download");
         header("Content-Disposition: attachment; filename=$fname");
         header("Content-Transfer-Encoding: binary");

         if(file_exists($fpath) && $fh = fopen($fpath, "rb"))
         {
             fseek($fh, $from);
             $cur_pos = ftell($fh);
             while($cur_pos !== FALSE && ftell($fh) + $bufsize < $to+1)
             {
                 $buffer = fread($fh, $bufsize);
                 print $buffer;
                 $cur_pos = ftell($fh);
             }

             $buffer = fread($fh, $to+1 - $cur_pos);
             print $buffer;

             fclose($fh);
         }
         else
         {
             header("HTTP/1.1 404 Not Found");
             exit;
         }
     }
     else
     {
         header("HTTP/1.1 500 Internal Server Error");
         exit;
     }
  }
  else // Usual download
  {
     header("HTTP/1.1 200 OK");
     header("Content-Length: $fsize");
     header("Content-Type: application/force-download");
     header("Content-Disposition: attachment; filename=$fname");
     header("Content-Transfer-Encoding: binary");

     if(file_exists($fpath) && $fh = fopen($fpath, "rb")){
         while($buf = fread($fh, $bufsize))
             print $buf;
         fclose($fh);
     }
     else
     {
         header("HTTP/1.1 404 Not Found");
         exit;
     }
  }
}
  
