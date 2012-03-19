# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

package Amanda::Archive;
use base qw(Exporter);
use base qw(DynaLoader);
package Amanda::Archivec;
bootstrap Amanda::Archive;
package Amanda::Archive;
@EXPORT = qw();

# ---------- BASE METHODS -------------

package Amanda::Archive;

sub TIEHASH {
    my ($classname,$obj) = @_;
    return bless $obj, $classname;
}

sub CLEAR { }

sub FIRSTKEY { }

sub NEXTKEY { }

sub FETCH {
    my ($self,$field) = @_;
    my $member_func = "swig_${field}_get";
    $self->$member_func();
}

sub STORE {
    my ($self,$field,$newval) = @_;
    my $member_func = "swig_${field}_set";
    $self->$member_func($newval);
}

sub this {
    my $ptr = shift;
    return tied(%$ptr);
}


# ------- FUNCTION WRAPPERS --------

package Amanda::Archive;

*amar_new = *Amanda::Archivec::amar_new;
*amar_close = *Amanda::Archivec::amar_close;
*amar_new_file = *Amanda::Archivec::amar_new_file;
*amar_file_close = *Amanda::Archivec::amar_file_close;
*amar_new_attr = *Amanda::Archivec::amar_new_attr;
*amar_attr_close = *Amanda::Archivec::amar_attr_close;
*amar_attr_add_data_buffer = *Amanda::Archivec::amar_attr_add_data_buffer;
*amar_attr_add_data_fd = *Amanda::Archivec::amar_attr_add_data_fd;
*amar_read = *Amanda::Archivec::amar_read;

# ------- VARIABLE STUBS --------

package Amanda::Archive;


@EXPORT_OK = ();
%EXPORT_TAGS = ();


=head1 NAME

Amanda::Archive - Perl access to the  amanda archive library

=head1 SYNOPSIS

  use Amanda::Archive

  # Write to the file descriptor or file handle $fd, and
  # add /etc/hosts to it
  my $archive = Amanda::Archive->new($fd, ">");
  my $file = $archive->new_file("/etc/hosts");
  my $attr = $file->new_attr(16);
  open(my $fh, "<", "/etc/hosts");
  $attr->add_data_fd($fh, 1);
  $file->close();
  $archive->close();

  # Read from an archive
  my $archive = Amanda::Archive->new($fd, "<");
  $ar->read(
      file_start => sub {
	  my ($user_data, $filenum, $filename) = @_;
	  # ...
	  return "foo"; # this becomes $file_data
      },
      file_finish => sub {
	  my ($user_data, $file_data, $filenum, $truncated) = @_;
	  # ...
      },
      21 => [ 32768,	# buffer into 32k chunks
	      sub {
		  my ($user_data, $filenum, $file_data, $attrid,
		      $attr_data, $data, $eoa, $truncated) = @_;
		  return "pants"; # becomes the new $attr_data for
				  # any subsequent fragments
	      } ],
      0 => sub {	# note no buffering here; attrid 0 is "default"
	  my ($user_data, $filenum, $file_data, $attrid,
	      $attr_data, $data, $eoa, $truncated) = @_;
	  return "shorts"; # becomes the new $attr_data for
			   # any subsequent fragments
      },
      user_data => [ "mydata" ], # sent to all callbacks
  );

=head1 WRITING

=head2 Amanda::Archive::Archive Objects

Note that C<< Amanda::Archive->new >> and C<<
Amanda::Archive::Archive->new >> are equivalent.

=over

=item C<new($fd, $mode)>

Create a new archive for reading ("<") or writing (">") from or to
file C<$fd> (a file handle or integer file descriptor).

=item C<new_file($filename, $want_posn)>

Create a new C<Amanda::Archive::File> object with the given filename
(writing only).  Equivalent to

  Amanda::Archive::File->new($archive, $filename, $want_posn);

if C<$want_posn> is false, then this method returns a new
C<Amanda::Archive::File> object.  If C<$want_posn> is true, then it
returns C<($file, $posn)> where C<$file> is the object and C<$posn> is
the offset into the datastream at which this file begins.  This offset
can be stored in an index and used later to seek into the file.

=item C<read(..)>

See I<READING>, below.

=item C<close()>

Flush all buffers and close this archive. This does not close the file
descriptor.

=back

=head2 Amanda::Archive::File Objects

=over

=item C<new($archive, $filename, $want_posn)>

Create a new file in the given archive.  See
C<Amanda::Archive::Archive::new_file>, above.

=item C<new_attr($attrid)>

Create a new C<Amanda::Archive::Attribute> object.  Equivalent to

  Amanda::Archive::Attr->new($file, $attrid);

=item C<close()>

Close this file, writing an EOF record.

=back

=head2 Amanda::Archive::Attribute Objects

=over

=item C<add_data($data, $eoa)>

Add C<$data> to this attribute, adding an EOA (end-of-attribute) bit
if C<$eoa> is true.

=item C<add_data_fd($fh, $eoa)>

Copy data from C<$fh> to this attribute, adding an EOA
(end-of-attribute) bit if C<$eoa> is true.

=item C<close()>

Close this attribute, adding an EOA bit if none has been written
already.

=back

=head1 READING

The C<Amanda::Archive::Archive> method C<read()> handles reading
archives via a callback mechanism.  It takes its arguments in hash
form, with the following keys:

    file_start => sub {
	my ($user_data, $filenum, $filename) = @_;
	# ..
    },

C<file_start> gives a sub which is called for every file in the
archive.  It can return an arbitrary value which will become the
C<$file_data> for subsequent callbacks in this file, or the string
"IGNORE" which will cause the reader to ignore all data for this file.
In this case, no other callbacks will be made for the file (not even
C<file_finish>).

    file_finish => sub {
	my ($user_data, $file_data, $filenum, $truncated) = @_;
	# ..
    },

C<file_finish> gives a sub which is called when an EOF record appears.
C<$file_data> comes from the return value of the C<file_start>
callback.  C<$truncated> is true if the file may be missing data
(e.g., when an early EOF is detected).

    user_data => $my_object,

C<user_data> gives an arbitrary value which is passed to each callback
as C<$user_data>.

    13 => sub {
	my ($user_data, $filenum, $file_data, $attrid,
	    $attr_data, $data, $eoa, $truncated) = @_;
	# ...
    },
    19 => [ 10240, sub { ... } ],

Any numeric key is treated as an attribute ID, and specifies the
handling for that attribute.  Attribute ID zero is treated as a
wildcard, and will match any attribute without an explicit handler.
The handler can be specified as a sub (as for attribute ID 13 in the
example above) or as an arrayref C<[$minsize, $sub]>.  In the latter
case, the sub is only called when at least C<$minsize> bytes of data
are available for the attribute, or at the end of the attribute data.

The parameters to the callback include C<$file_data>, the value
returned from C<file_start>, and C<$attr_data>, which is the return
value of the last invocation of this sub for this attribute.  If this
is the last fragment of data for this attribute, then C<$eoa> is true.
The meaning of C<$truncated> is similar to that in C<file_finish>.

=head2 EXAMPLE

    sub read_to_files {
	my ($arch_fh, $basedir) = @_;

	my $arch = Amanda::Archive->new(fileno($arch_fh), "<");
	$arch->read(
	    file_start => sub {
		my ($user_data, $filenum, $filename) = @_;
		return "$basedir/$filenum"; # becomes $file_data
	    },
	    0 => [ 32768, sub {
		my ($user_data, $filenum, $file_data, $attrid,
		    $attr_data, $data, $eoa, $truncated) = @_;
		warn("file $filename attribute $attrid is truncated")
		    if ($truncated);
		# store the open filehandle in $attr_data
		if (!$attr_data) {
		    open($attr_data, "$file_data.$attrid", ">")
			or die("open: $!");
		}
		print $attr_data $data;
		if ($eoa) {
		    close($attr_data);
		}
		return $attr_data;
	    },
	);
    }

=cut



package Amanda::Archive;

# Expose the Archive constructor at Amanda::Archive->new
sub new {
    my $pkg = shift;
    Amanda::Archive::Archive->new(@_);
}

package Amanda::Archive::Archive;

sub new {
    my ($class, $fd, $mode) = @_;
    my $arch = Amanda::Archive::amar_new($fd, $mode);
    return bless (\$arch, $class);
}

sub close {
    my $self = shift;
    if ($$self) {
	Amanda::Archive::amar_close($$self);
	$$self = undef;
    }
}

sub DESTROY {
    my $self = shift;
    $self->close();
}

sub new_file {
    my ($self, $filename, $want_offset) = @_;
    return Amanda::Archive::File->new($self, $filename, $want_offset);
}

sub Amanda::Archive::Archive::read {
    my $self = shift;
    die "Archive is not open" unless ($$self);
    # pass a hashref to the C code
    my %h = @_;
    Amanda::Archive::amar_read($$self, \%h);
}

package Amanda::Archive::File;

sub new {
    my ($class, $arch, $filename, $want_offset) = @_;
    die "Archive is not open" unless ($$arch);
    if ($want_offset) {
	# note that posn is returned first by the SWIG wrapper
	my ($file, $posn) = Amanda::Archive::amar_new_file($$arch, $filename, $want_offset);
	return (bless([ $file, $arch ], $class), $posn);
    } else {
	my $file = Amanda::Archive::amar_new_file($$arch, $filename, $want_offset);
	return bless([ $file, $arch ], $class);
    }
}

sub close {
    my $self = shift;
    if ($self->[0]) {
	Amanda::Archive::amar_file_close($self->[0]);
	$self->[0] = undef;
    }
}

sub DESTROY {
    my $self = shift;
    $self->close();
}

sub new_attr {
    my ($self, $attrid) = @_;
    return Amanda::Archive::Attr->new($self, $attrid);
}

package Amanda::Archive::Attr;

sub new {
    my ($class, $file, $attrid) = @_;
    die "File is not open" unless ($file->[0]);
    my $attr = Amanda::Archive::amar_new_attr($file->[0], $attrid);
    return bless ([$attr, $file], $class);
}

sub close {
    my $self = shift;
    if ($self->[0]) {
	Amanda::Archive::amar_attr_close($self->[0]);
	$self->[0] = undef;
    }
}

sub DESTROY {
    my $self = shift;
    $self->close();
}

sub add_data {
    my ($self, $data, $eoa) = @_;
    die "Attr is not open" unless ($self->[0]);
    Amanda::Archive::amar_attr_add_data_buffer($self->[0], $data, $eoa);
}

sub add_data_fd {
    my ($self, $fd, $eoa) = @_;
    die "Attr is not open" unless ($self->[0]);
    return Amanda::Archive::amar_attr_add_data_fd($self->[0], $fd, $eoa);
}
1;
