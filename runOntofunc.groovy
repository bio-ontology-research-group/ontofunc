import groovy.sql.Sql
import javax.mail.*
import javax.mail.internet.*

def basedir = "/usr/local/bin/"
sql = Sql.newInstance("jdbc:mysql://localhost:3306/ontofunc?connectTimeout=0&socketTimeout=0", "root","", "com.mysql.jdbc.Driver")

errorLine = null // global error message variable
errorCode = 0 // 1 means number of arguments is wrong, 2 means number format is wrong
encounteredAt = -1

Boolean checkInput(File f, String type) {
  def counter = 0
  def retVal = true
  if (type=="binomial") {
    f.splitEachLine("\t") { line ->
      if (errorCode==0) {
	counter += 1
	if (line.size()!=4) {
	  println line
	  errorLine = line
	  errorCode = 1
	  encounteredAt = counter
	  retVal = false
	} else {
	  try {
	    Double.parseDouble(line[2])
	    Double.parseDouble(line[3])
	  } catch (Exception E) {
	    errorCode = 2
	    errorLine = line
	    encounteredAt = counter
	    retVal = false
	  }
	}
      }
    }
    return retVal
  }
  if (type=="hypergeometric") {
    f.splitEachLine("\t") { line ->
      if (errorCode==0) {
	counter += 1
	if (line.size()!=3) {
	  errorLine = line
	  errorCode = 1
	  encounteredAt = counter
	  retVal = false
	} else if (line[2]!="0" && line[2]!="1") {
	  errorCode = 2
	  errorLine = line
	  encounteredAt = counter
	  retVal = false
	}
      }
    }
    return retVal
  }
  if (type=="wilcoxon") {
    f.splitEachLine("\t") { line ->
      if (errorCode==0) {
	counter += 1
	if (line.size()!=3) {
	  errorLine = line
	  errorCode = 1
	  encounteredAt = counter
	  retVal = false
	} else {
	  try {
	    Double.parseDouble(line[2])
	  } catch (Exception E) {
	    errorCode = 2
	    errorLine = line
	    encounteredAt = counter
	    retVal = false
	  }
	}
      }
    }
    return retVal
  }
  if (type=="mcdonaldkreitman") {
    f.splitEachLine("\t") { line ->
      if (errorCode==0) {
	counter += 1
	if (line.size()!=6) {
	  errorLine = line
	  errorCode = 1
	  encounteredAt = counter
	  retVal = false
	} else {
	  try {
	    Double.parseDouble(line[2])
	    Double.parseDouble(line[3])
	    Double.parseDouble(line[4])
	    Double.parseDouble(line[5])
	  } catch (Exception E) {
	    errorCode = 2
	    errorLine = line
	    encounteredAt = counter
	    retVal false
	  }
	}
      }
    }
    return retVal
  }
}

def datadir = "/home/leechuck/Public/software/ontofunc/ontofunc/data/"
def outdatadir = "/home/leechuck/Public/software/ontofunc/ontofunc/dataout/"
def structuredir = "/home/leechuck/Public/software/ontofunc/ontofunc/structures/"
def basedatadir = "/home/leechuck/Public/software/ontofunc/ontofunc/obodata/"

def id = args[0]
def query = "SELECT * FROM Job WHERE id='$id'" 

sql.eachRow(query) { 
  try {
    def infile = datadir+id+"/input.txt"
    def outdir = outdatadir+id
    new File(outdir).mkdir()
    def ont = it.ontology
    def version = it.version
    def termdir = basedatadir+version+"/structures/"+ont
    def cutoff = it.cutoff
    def jobname = it.name
    def rootnode = it.rootnode
    def randomsets = it.randomsets
    def email = it.email
    switch (it.test) {
    case "binomial":
      cmd = basedir+"func_binom -i $infile -t $termdir -o $outdir -c $cutoff -g $rootnode -r $randomsets"
      break ;
    case "wilcoxon":
      cmd = basedir+"func_wilcoxon -i $infile -t $termdir -o $outdir -c $cutoff -g $rootnode -r $randomsets"
      break ;
    case "hypergeometric":
      cmd = basedir+"func_hyper -i $infile -t $termdir -o $outdir -c $cutoff -g $rootnode -r $randomsets"
      break ;
    case "mcdonaldkreitman":
      cmd = basedir+"func_2x2contingency -i $infile -t $termdir -o $outdir -c $cutoff -g $rootnode -r $randomsets"
      break ;
    }
    if (checkInput(new File(infile), it.test) == false || cmd.length()<5) {
      sql.execute("UPDATE Job SET status=3 WHERE id=$id")
      sql.execute("UPDATE Job SET errorcode=$errorCode WHERE id=$id")
      def errorMessage = ""
      errorLine.each { errorMessage += it+"\t" }
      sql.execute("UPDATE Job SET errorline='"+errorMessage+"' WHERE id=$id")
      sql.execute("UPDATE Job SET encountered=$encounteredAt WHERE id=$id")
    } else {
      sql.execute("UPDATE Job SET status=1 WHERE id=$id")
      println cmd
      def proc = cmd.execute()
      proc.consumeProcessOutput() // dump the output of the process
      
      proc.waitForOrKill(18000*1000) // 5 h max job execution time
      def result = proc.exitValue()
      def messageText = ""
      if (result == 0) {
	messageText = "Hi. Your OntoFUNC job \"$jobname\" with id $id has finished. To view your results, please visit http://phenomebrowser.net/ontofunc/viewresults.php?id=$id. For questions and help, please respond to this email.\n\nRobert Hoehndorf"
	sql.execute("UPDATE Job SET status=2 WHERE id=$id") // executed ok
      } else {
	messageText = "Hi. Your OntoFUNC job \"$jobname\" with id $id has finished unsuccessfully. Please check that your input file is formatted correctly for the test you are running. The error message on http://phenomebrowser.net/ontofunc/checkstatus.php?id=$id may provide further help. For questions and help, please respond to this email.\n\nRobert Hoehndorf"
	sql.execute("UPDATE Job SET status=3 WHERE id=$id") // error status
      }
      def properties = new Properties ( ) 
      properties.put ( 'mail.smtp.host' , 'smtphost.aber.ac.uk' ) 
      def session = Session.getInstance ( properties ) 
      try { 
	def message = new MimeMessage ( session ) 
	message.from = new InternetAddress ( 'roh25@aber.ac.uk' )
	message.setRecipient ( Message.RecipientType.TO , new InternetAddress ( email ) )
	message.subject = "Your OntoFUNC Job with id $id has finished" 
	message.sentDate = new Date ( ) 
	message.text = messageText
	Transport.send ( message ) 
      } 
      catch ( final MessagingException me ) { 
	me.printStackTrace ( ) 
      } 
    }
  } catch (Exception E) {
    E.printStackTrace()
    sql.execute("UPDATE Job SET status=3 WHERE id=$id")
    sql.execute("UPDATE Job SET errorcode=$errorCode WHERE id=$id")
  }
}